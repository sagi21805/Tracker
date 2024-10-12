use pyo3::pymethods;

use crate::bounding_box::BoundingBox;
use crate::config::Config;
use crate::constants::*;
use crate::entity_state::EntityState;
use crate::point::Point;
use crate::rect::Rect;
use pyo3::prelude::*;
use std::collections::LinkedList;
use std::sync::atomic::{AtomicU16, Ordering};

#[pyclass]
#[derive(Clone)]
pub struct Entity {
    #[pyo3(get)]
    pub(crate) id: u16,
    #[pyo3(get)]
    pub(crate) bounding_box: BoundingBox,
    #[pyo3(get)]
    pub(crate) predicted_location: Rect,
    #[pyo3(get)]
    pub(crate) times_not_found: u32,

    pub(crate) velocity: Velocity2D,
    pub(crate) trajectory: LinkedList<EntityState>,
}

#[pymethods]
impl Entity {
    #[new]
    pub fn new(bounding_box: BoundingBox) -> Self {
        static STARTING_ID: AtomicU16 = AtomicU16::new(0);
        let id = STARTING_ID.fetch_add(1, Ordering::SeqCst);

        let mut entity = Entity {
            id,
            bounding_box,
            velocity: Velocity2D::new(0, 0),
            trajectory: LinkedList::<EntityState>::new(),
            predicted_location: Rect::default(),
            times_not_found: 0,
        };

        entity.trajectory.push_front(EntityState::new(
            entity.bounding_box.clone(),
            Velocity2D::new(0, 0),
        ));
        return entity;
    }
}

impl Entity {
    pub fn calc_and_set_velocity(&mut self) {
        // TODO very ugly improve
        if self.trajectory.len() >= 2 {
            let start_point = self.trajectory.iter().nth(1).unwrap();
            let vel =
            self.bounding_box.rect.get_center() - start_point.bounding_box.rect.get_center() ;
            self.velocity = Velocity2D::new(vel.x, vel.y);
        }
    }

    pub fn predict_next_bounding_box(&mut self) {
        self.bounding_box.rect.x += self.velocity.x;
        self.bounding_box.rect.y += self.velocity.y;
    }

    pub fn square_distance_to(&self, e: &Entity) -> i32 {
        self.bounding_box
            .rect
            .square_distance_to(&e.bounding_box.rect)
    }

    //TODO get rid of magic numbers
    pub fn predict_possible_locations(&mut self, config: &Config) {
        let r = self.bounding_box.rect.clone();
        let vx = self.velocity.x as f32 * config.vel_coefficient;
        let vy = self.velocity.y as f32 * config.vel_coefficient;
        let w =
            r.width as f32 * config.size_coefficient /* vx.signum()*/ * self.times_not_found as f32;
        let h =
            r.height as f32 * config.size_coefficient /* vy.signum()*/ * self.times_not_found as f32;

        let mut tl = r.get_center() - Point::new(w as i32, h as i32);
        let mut br = r.get_center() + Point::new(w as i32, h as i32);


        if vx > 0.0 { br.x += vx as i32 } else { tl.x += vx as i32 }
        if vy > 0.0 { br.y += vy as i32 } else { tl.y += vy as i32 }


        self.predicted_location = Rect::from_points(
            tl, br
        );
    }

    // TODO very ugly improve and avoid magic numbers
    pub fn calc_score(&self, matched_prediction: &BoundingBox) -> f32 {
        if self.bounding_box.rect.is_empty() {
            return 0.0;
        }
        if self.bounding_box.group_id != matched_prediction.group_id {
            return 0.0;
        }
        let mut score = 0.0;
        score += if self
            .predicted_location
            .contains(matched_prediction.rect.get_center())
        {
            0.15
        } else {
            0.0
        };
        score += if self
            .predicted_location
            .contains(matched_prediction.rect.tl())
        {
            0.1
        } else {
            0.0
        };
        score += if self
            .predicted_location
            .contains(matched_prediction.rect.br())
        {
            0.1
        } else {
            0.0
        };

        score += matched_prediction
            .rect
            .iou_percentage(&self.predicted_location)
            * 0.15;

        let distance =
            (self.predicted_location.width.pow(2) + self.predicted_location.height.pow(2)) as f32;
        let slope = (0.75 - 1.0) / distance;
        let current_distance = self
            .bounding_box
            .rect
            .square_distance_to(&matched_prediction.rect) as f32;
        score += f32::max(0.5 * (slope * current_distance + 1.0), 0.0);
        score
    }
}

impl std::fmt::Display for Entity {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(
            f,
            "id: {}\ntype: {:?}\nbox: {}\n, not_found: {}",
            self.id, self.bounding_box.group_id, self.bounding_box.rect, self.times_not_found
        )
    }
}
