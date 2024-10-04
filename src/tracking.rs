// use opencv::{core, highgui, imgproc, prelude::*};
// use serde_json::from_str;
// use std::sync::Arc;
use crate::constants::{MIN_CONFIDENCE, MIN_SCORE, MOVE_TO_LAST_SEEN, RATIO, TO_VISUALIZE};
use crate::entity::Entity;
use crate::entity_state::EntityState;
use crate::rect::Rect;
use crate::{bounding_box::BoundingBox, constants::ELEMENTS_IN_POINT};
use numpy::*;
use pyo3::prelude::*;
use std::collections::LinkedList;

#[pyclass]
pub struct _Tracker {
    // pub(crate) current_recognition: Vec<BoundingBox>,
    pub(crate) entities: LinkedList<Entity>,
    pub(crate) last_seen: LinkedList<Entity>,
}

impl _Tracker {

    fn generate_boxes(
        points: PyReadonlyArray1<i32>,
        classes: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
    ) -> Vec<BoundingBox> {
        let mut bounding_boxes = Vec::with_capacity(points.len());
        let points_slice = points.as_slice().expect("array is not contigous");
        let confidences_slice = confidences.as_slice().expect("array is not contigous");
        let classes_slice = classes.as_slice().expect("array is not contigous");

        for ((point, confidence), class) in points_slice
            .chunks(ELEMENTS_IN_POINT)
            .zip(confidences_slice.iter())
            .zip(classes_slice.iter())
        {
            if *confidence > MIN_CONFIDENCE {
                bounding_boxes.push(BoundingBox {
                    rect: Rect {
                        x: point[0],
                        y: point[1],
                        width: point[2],
                        height: point[3],
                    },
                    class: *class,
                    confidence: *confidence,
                });
            }
        }
        return bounding_boxes;
    }

    pub fn match_entity(&mut self, recognition: &mut Vec<BoundingBox>) {
        println!("length: {}", self.entities.len());
        for rec in recognition.clone() {
            println!("BoundingBoxes: {}", rec);
        }
        let _ = recognition.iter_mut().map(|matched_entity| {

            let (mut existing, max_score) = self.entities
            .iter_mut()
            .map(|existing_entity| {
                (existing_entity.clone(), existing_entity.calc_score(matched_entity))
            })
            .max_by(|(_, score_x), (_, score_y)| score_x.partial_cmp(score_y).unwrap())
            .expect("There are no entities to compare");

            if max_score > MIN_SCORE {

                if existing.found_recognition {
                    existing.bounding_box.merge(matched_entity, RATIO);
                } else {
                    existing.bounding_box.combine(matched_entity);
                    existing.found_recognition = true;
                }
            } else if matched_entity.confidence > MIN_CONFIDENCE {
                self.entities.push_back(Entity::new(matched_entity.clone()));
            }

        });

    }


    pub fn start_cycle(
        &mut self
    ) {
        for entity in self.entities.iter_mut() {
            entity.calc_and_set_velocity();
            entity.predict_possible_locations();
        }
    }

    // TODO waste of performance can do better in different context
    pub fn manage_last_seen(&mut self, remaining_recognition: &mut Vec<BoundingBox>) {
        if !self.last_seen.is_empty() {
            self.match_entity(remaining_recognition);
            let _ = self.last_seen.iter_mut().map(|entity| {
                if entity.times_not_found > 60 {
                    false
                } else if entity.found_recognition {
                    self.entities.push_front(entity.clone());
                    false
                } else {
                    entity.times_not_found += 1;
                    true
                }
            });
        }
    }

    pub fn manage_entities(&mut self, remaining_recognition: &mut Vec<BoundingBox>) {
        let _ = self.entities.iter_mut().map(|entity| {
            if TO_VISUALIZE {
                // entity.draw(&mut self.frame);
                // entity
                //     .get_possible_location()
                //     .draw(&mut self.frame, core::Scalar::new(255.0, 255.0, 255.0, 0.0));
            }
            
            let state = EntityState::new(entity.bounding_box.clone(), entity.velocity);
            entity.trajectory.push_front(state);

            if entity.found_recognition {
                entity.found_recognition = false;
                entity.times_not_found = 0;
            } else {
                entity.predict_next_bounding_box();
                entity.times_not_found += 1;
            }

            if entity.times_not_found >= MOVE_TO_LAST_SEEN {
                self.last_seen.push_front(entity.clone());
            } 
        });
        self.manage_last_seen(remaining_recognition);
    }

    pub fn end_cycle(&mut self, remaining_recognition: &mut Vec<BoundingBox>) {
        self.manage_entities(remaining_recognition);

        // if visualization::TO_VISUALIZE {
        //     highgui::imshow("frame", &self.frame).unwrap();
        //     highgui::wait_key(visualization::WAIT_KEY).unwrap();
        // }
    }
}

#[pyclass]
pub struct Simple {
    #[pyo3(get)]
    id: u16,
    #[pyo3(get)]
    bounding_box: BoundingBox 
}

#[pymethods]
impl Simple {

    #[new]
    pub fn new(id: u16, bounding_box: BoundingBox) -> Self{
        Simple {
            id, bounding_box
        }
    }
}


#[pymethods]
impl _Tracker {

    #[new]
    pub fn new() -> Self {
        _Tracker {
            // current_recognition: Vec::new(),
            entities: LinkedList::new(),
            last_seen: LinkedList::new(),
        }
    }

    pub fn track(
        &mut self,
        points: PyReadonlyArray1<i32>,
        classes: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
    ) -> Vec<Simple> { 
        let mut current_recognition = _Tracker::generate_boxes(points, classes, confidences);

        self.start_cycle();
        self.match_entity(&mut current_recognition);
        self.end_cycle(&mut current_recognition); //TODO should give remaining one

        return self.entities.iter().map(|entity| Simple::new(entity.id, entity.bounding_box.clone())).collect()
    }
}
