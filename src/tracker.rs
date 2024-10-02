// use opencv::{core, highgui, imgproc, prelude::*};
// use serde_json::from_str;
// use std::sync::Arc;
use crate::constants::{MIN_CONFIDENCE, RATIO};
use crate::entity::Entity;
use crate::rect::Rect;
use crate::{bounding_box::BoundingBox, constants::ELEMENTS_IN_POINT};
use numpy::*;
use pyo3::prelude::*;
use std::collections::LinkedList;

#[pyclass]
pub struct Tracker {
    pub(crate) current_recognition: Vec<BoundingBox>,
    pub(crate) entities: LinkedList<Entity>,
    pub(crate) last_seen: LinkedList<Entity>,
}

impl Tracker {
    pub fn new() -> Self {
        Tracker {
            current_recognition: Vec::new(),
            entities: LinkedList::new(),
            last_seen: LinkedList::new(),
        }
    }

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
        
        self.entities.iter_mut().map(|existing_entity| {

            let (max_index, matching) = recognition
            .iter()
            .enumerate() // Pair each item with its index
            .max_by(|(_, x), (_, y)| {
                existing_entity
                    .calc_score(x)
                    .partial_cmp(&existing_entity.calc_score(y))
                    .unwrap()
            })
            .expect("There are no entities to compare");

            if existing_entity.found_recognition {
                existing_entity.bounding_box.combine(matching);
            } else {
                existing_entity.bounding_box.merge(matching, RATIO);
                existing_entity.found_recognition = true;
            }
            recognition.swap_remove(max_index);
        });
    }


    pub fn start_cycle(
        &mut self,
        points: &[i32],
        types: &[u16],
        confidences: &[f32],
        size: u16,
        frame: &[u8],
    ) {
        self.set_current_recognition(points, types, confidences, size, frame);
        for entity in self.entities.iter_mut() {
            entity.calc_and_set_velocity();
            entity.predict_possible_locations();
        }
    }

    pub fn manage_last_seen(&mut self) {
        println!("length: {}", self.last_seen.len());
        if !self.last_seen.is_empty() {
            self.match_entity(
                &mut self.last_seen,
                &mut self.current_recognition,
                core::MIN_SCORE / 2.0,
            );
            self.last_seen.retain(|entity| {
                if entity.times_not_found > 60 {
                    false
                } else if entity.found_recognition {
                    self.entities.prepend(entity.clone());
                    false
                } else {
                    entity.times_not_found += 1;
                    true
                }
            });
        }
    }

    pub fn manage_entities(&mut self) {
        self.entities.retain(|entity| {
            if visualization::TO_VISUALIZE {
                entity.draw(&mut self.frame);
                entity
                    .get_possible_location()
                    .draw(&mut self.frame, core::Scalar::new(255.0, 255.0, 255.0, 0.0));
            }
            entity.add_to_EntityState();
            if entity.found_recognition {
                entity.found_recognition = false;
                entity.times_not_found = 0;
            } else {
                entity.predict_next_bounding_box();
                entity.times_not_found += 1;
            }

            if entity.times_not_found >= core::MOVE_TO_LAST_SEEN {
                self.last_seen.prepend(entity.clone());
                false
            } else {
                true
            }
        });
        self.manage_last_seen();
    }

    pub fn end_cycle(&mut self) {
        self.manage_entities();
        self.generate_entities();

        if visualization::TO_VISUALIZE {
            highgui::imshow("frame", &self.frame).unwrap();
            highgui::wait_key(visualization::WAIT_KEY).unwrap();
        }
    }
}

#[pymethods]
impl Tracker {
    pub fn track(
        &mut self,
        points: PyReadonlyArray1<i32>,
        classes: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
        frame: Py<PyArray3<u8>>,
    ) {
        let current_recognition = Tracker::generate_boxes(points, classes, confidences);

        self.start_cycle(points, types, confidences, size, frame);
        self.match_entity(
            &mut self.entities,
            &mut self.current_recognition,
            core::MIN_SCORE,
        );
        self.end_cycle();
    }
}

impl Drop for Tracker {
    fn drop(&mut self) {
        highgui::destroy_all_windows().unwrap();
    }
}
