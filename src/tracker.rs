use opencv::{core, highgui, imgproc, prelude::*};
use serde_json::from_str;
use std::sync::Arc;

pub struct Tracker {
    pub(crate) rows: u16,
    pub(crate) cols: u16,
    pub(crate) current_recognition: Vec<BoundingBox>,
    pub(crate) frame: Vec<u8>, //Will be changed to a python type
    pub(crate) entities: LinkedList<Entity>,
    pub(crate) last_seen: LinkedList<Entity>,
}

impl Tracker {
    pub fn new(points: &[i32], types: &[u16], confidences: &[f32], size: u16, frame: &[u8], rows: u16, cols: u16) -> Self {
        let mut tracker = Tracker {
            rows,
            cols,
            current_recognition: Vec::new(),
            frame: Mat::default(),
            entities: LinkedList::new(),
            last_seen: LinkedList::new(),
        };
        
        tracker.config("config.json");
        tracker.set_current_recognition(points, types, confidences, size, frame);
        tracker.generate_entities();
        tracker
    }

    pub fn set_current_recognition(&mut self, points: &[i32], types: &[u16], confidences: &[f32], size: u16, frame: &[u8]) {
        self.current_recognition = generate_bounding_boxes(points, types, confidences, size);
        self.set_frame(frame);
    }

    pub fn set_frame(&mut self, frame: &[u8]) {
        self.frame = Mat::from_slice_rows_cols(frame, self.rows as i32, self.cols as i32, core::CV_8UC3).unwrap();
    }

    pub fn match_entity(&mut self, matched_list_of_entities: &mut LinkedList<Entity>, recognition: &mut Vec<BoundingBox>, min_score: f32) {
        for i in (0..recognition.len()).rev() {
            let mut max_score = 0.0;
            let mut matched_entity_ptr = None;
            
            for entity in matched_list_of_entities.iter_mut() {
                let current_score = entity.calc_score(&recognition[i]);
                if current_score > max_score {
                    max_score = current_score;
                    matched_entity_ptr = Some(entity);
                }
            }

            if max_score > min_score {
                if let Some(matched_entity) = matched_entity_ptr {
                    if matched_entity.found_recognition {
                        matched_entity.combine_bounding_box(&recognition[i]);
                    } else {
                        matched_entity.set_bounding_box(recognition[i].clone());
                    }
                    matched_entity.found_recognition = true;
                    recognition.remove(i);
                }
            }
        }
    }

    pub fn generate_entities(&mut self) {
        for box_ in &self.current_recognition {
            self.entities.prepend(Entity::new(box_.clone()));
        }
    }

    pub fn start_cycle(&mut self, points: &[i32], types: &[u16], confidences: &[f32], size: u16, frame: &[u8]) {
        self.set_current_recognition(points, types, confidences, size, frame);
        for entity in self.entities.iter_mut() {
            entity.calc_and_set_velocity();
            entity.predict_possible_locations();
        }
    }

    pub fn manage_last_seen(&mut self) {
        println!("length: {}", self.last_seen.len());
        if !self.last_seen.is_empty() {
            self.match_entity(&mut self.last_seen, &mut self.current_recognition, core::MIN_SCORE / 2.0);
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
                entity.get_possible_location().draw(&mut self.frame, core::Scalar::new(255.0, 255.0, 255.0, 0.0));
            }
            entity.add_to_trajectory();
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

    pub fn track(&mut self, points: &[i32], types: &[u16], confidences: &[f32], size: u16, frame: &[u8]) {
        self.start_cycle(points, types, confidences, size, frame);
        self.match_entity(&mut self.entities, &mut self.current_recognition, core::MIN_SCORE);
        self.end_cycle();
    }
}

impl Drop for Tracker {
    fn drop(&mut self) {
        highgui::destroy_all_windows().unwrap();
    }
}

