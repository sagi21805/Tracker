use crate::bounding_box::BoundingBox;
use crate::entity::Entity;
use crate::entity_state::EntityState;
use crate::general_tracker::GeneralTracker;
use crate::{config::Config, general_tracker::TrackerUtils};
use numpy::*;
use pyo3::{pyclass, pymethods};

#[pyclass]
pub struct DynamicEntityTracker {
    pub(crate) entities: Vec<Entity>,
    pub(crate) config: Config,
}

impl TrackerUtils for DynamicEntityTracker {
    fn get_config(&self) -> &Config {
        return &self.config;
    }

    fn match_entity(&mut self, recognition: &mut Vec<BoundingBox>) {
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
                println!("Max Score: {}, for: {}", max_score, existing.id);
                if max_score > self.config.min_score {
                    existing
                        .bounding_box
                        .merge(matched_entity, self.config.ratio);
                    existing.times_not_found = 0;
                    return false;
                }
            }
            self.entities.push(Entity::new(matched_entity.clone()));
            std::thread::sleep(std::time::Duration::from_millis(500));
            println!("############################Sleeping###########################");
            return true;
        });
    }

    fn manage_entities(&mut self) {
        self.entities.retain_mut(|entity| {
            entity.trajectory.push_front(EntityState::new(
                entity.bounding_box.clone(),
                entity.velocity,
            ));
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

    fn start_cycle(&mut self) {
        for entity in self.entities.iter_mut() {
            entity.calc_and_set_velocity();
            entity.predict_possible_locations(&self.config);
        }
    }
}

impl GeneralTracker for DynamicEntityTracker {
    fn track_entities(
        &mut self,
        points: PyReadonlyArray1<i32>,
        typees: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
    ) -> Vec<Entity> {
        let mut current_recognition = self.generate_boxes(points, typees, confidences);

        self.start_cycle();
        self.match_entity(&mut current_recognition); // This function removes the matched entities from the recognition
        self.manage_entities();
        return self.entities.iter().map(|entity| entity.clone()).collect();
    }
}

#[pymethods]
impl DynamicEntityTracker {
    #[new]
    fn new(config_path: &str) -> Self {
        DynamicEntityTracker {
            entities: Vec::new(),
            config: Config::from_json_file(config_path).expect("Can't load configuration"),
        }
    }

    fn track(
        &mut self,
        points: PyReadonlyArray1<i32>,
        typees: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
    ) -> Vec<Entity> {
        self.track_entities(points, typees, confidences)
    }
}
