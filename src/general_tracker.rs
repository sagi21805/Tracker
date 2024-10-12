use crate::config::Config;
use crate::entity::Entity;
use crate::entity_state::EntityState;
use crate::point::Point;
use crate::rect::Rect;
use crate::bounding_box::BoundingBox;
use numpy::*;
use pyo3::prelude::*;

#[pyclass]
pub struct GeneralTracker {
    pub(crate) entities: Vec<Entity>,
    pub(crate) config: Config,
}

impl GeneralTracker {
    fn generate_boxes(
        &self,
        points: PyReadonlyArray1<i32>,
        typees: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
    ) -> Vec<BoundingBox> {
        let mut bounding_boxes = Vec::with_capacity(points.len());
        let points_slice = points.as_slice().expect("array is not contigous");
        let confidences_slice = confidences.as_slice().expect("array is not contigous");
        let typees_slice = typees.as_slice().expect("array is not contigous");

        for ((point, confidence), group_id) in points_slice
            .chunks(self.config.elements_in_point)
            .zip(confidences_slice.iter())
            .zip(typees_slice.iter())
        {
            if *confidence > self.config.min_confidence {
                bounding_boxes.push(BoundingBox {
                    rect: Rect::from_points(
                        Point::new(point[0], point[1]),
                        Point::new(point[2], point[3]),
                    ),
                    group_id: *group_id,
                    confidence: *confidence,
                });
            }
        }
        return bounding_boxes;
    }

    pub fn match_entity(&mut self, recognition: &mut Vec<BoundingBox>) {
        recognition.retain_mut(|matched_entity| {
            if let Some((max_score, existing)) = self
                .entities
                .iter_mut()
                .map(|existing_entity| {
                    (
                        existing_entity.clone().calc_score(matched_entity),
                        existing_entity,
                    )
                })
                .max_by(|(score_x, _), (score_y, _)| score_x.partial_cmp(score_y).unwrap())
            {
                // Your logic when there is a max score
                if max_score > self.config.min_score {
                    existing.bounding_box.merge(matched_entity, self.config.ratio);
                    existing.times_not_found = 0;
                    return false;
                }
            }
            self.entities.push(Entity::new(matched_entity.clone()));
            return true;
        });
    }

    pub fn start_cycle(&mut self) {
        for entity in self.entities.iter_mut() {
            entity.calc_and_set_velocity();
            entity.predict_possible_locations(&self.config);
        }
    }

 

    pub fn manage_entities(&mut self) {
        self.entities.retain_mut(|entity| {
            let state = EntityState::new(
                entity.bounding_box.clone(), entity.velocity
            );
            entity.trajectory.push_front(state);
            entity.times_not_found += 1;
            if entity.times_not_found >= 3 {
                entity.predict_next_bounding_box();
            }
            if entity.times_not_found > self.config.move_to_last_seen {
                return false;
            }
            return true;

        });
    }
}


#[pymethods]
impl GeneralTracker {
    #[new]
    pub fn new(config_path: &str) -> Self {
        GeneralTracker {
            entities: Vec::new(),
            config: Config::from_json_file(config_path).expect("Can't load configuration")
        }
    }

    pub fn track(
        &mut self,
        points: PyReadonlyArray1<i32>,
        typees: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
    ) -> Vec<Entity> {
        let mut current_recognition = self.generate_boxes(points, typees, confidences);

        self.start_cycle();
        self.match_entity(&mut current_recognition); // This function removes the matched entities from the recognition
        self.manage_entities();
        return self
            .entities
            .iter()
            .map(|entity| {
                entity.clone()
            })
            .collect();
    }
}
