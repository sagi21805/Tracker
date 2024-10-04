use pyo3::pymethods;

use crate::bounding_box::BoundingBox;
use crate::constants::*;
use crate::entity_state::EntityState;
use crate::rect::Rect;
use std::collections::LinkedList;
use std::sync::atomic::{AtomicU16, Ordering};
use pyo3::prelude::*;

#[pyclass]
#[derive(Clone)]
pub struct Entity {
    pub(crate) id: u16,
    pub(crate) bounding_box: BoundingBox,
    pub(crate) velocity: Velocity2D,
    pub(crate) trajectory: LinkedList<EntityState>,
    pub(crate) possible_location: Rect,
    pub(crate) times_not_found: u64,
    pub(crate) found_recognition: bool,
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
            possible_location: Rect::default(),
            times_not_found: 0,
            found_recognition: false,
        };

        entity.trajectory.push_front(EntityState::new(
            entity.bounding_box.clone(),
            Velocity2D::new(0, 0),
        ));
        return entity;
    }

}

impl Entity {
    

    fn choose_color() -> [u8; 3] {
        // Implement color choosing logic here
        [0, 0, 0]
    }

    pub fn calc_and_set_velocity(&mut self) {

        // TODO very ugly improve
        if self.trajectory.len() >= 1 {
            let end_point = self.trajectory.iter().nth(1).unwrap();
            let vel = end_point.bounding_box.rect.get_center() - self.bounding_box.rect.get_center();
            self.velocity = Velocity2D::new(
                vel.x,
                vel.y
            );
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
    pub fn predict_possible_locations(&mut self) {
        let r = self.bounding_box.rect.clone();
        let vx = self.velocity.x as f32 * VEL_COEFFICIENT;
        let vy = self.velocity.y as f32 * VEL_COEFFICIENT;
        let w =
            r.width as f32 * SIZE_COEFFICIENT * vx.signum() * (self.times_not_found as f32 + 5.0)
                / 5.0;
        let h =
            r.height as f32 * SIZE_COEFFICIENT * vy.signum() * (self.times_not_found as f32 + 5.0)
                / 5.0;
        self.possible_location = Rect::from_points(
            r.get_center() + Point::new(vx as i32 + w as i32, vy as i32 + h as i32),
            r.get_center() - Point::new(w as i32, h as i32),
        );
    }

    // pub fn draw(&self, frame: &mut Mat) {
    //     self.get_bounding_box().rect.draw(frame, self.color);
    //     imgproc::put_text(
    //         frame,
    //         &self.id.to_string(),
    //         self.bounding_box.rect.tl(),
    //         imgproc::FONT_HERSHEY_DUPLEX,
    //         1.0,
    //         core::Scalar::new(255.0, 255.0, 0.0, 0.0),
    //         2,
    //         imgproc::LINE_8,
    //         false,
    //     ).unwrap();
    //     imgproc::put_text(
    //         frame,
    //         &format!("{:.2}", self.bounding_box.confidence),
    //         self.bounding_box.rect.center - Point::new(24, -12),
    //         imgproc::FONT_HERSHEY_DUPLEX,
    //         1.0,
    //         core::Scalar::new(0.0, 255.0, 255.0, 0.0),
    //         2,
    //         imgproc::LINE_8,
    //         false,
    //     ).unwrap();
    // }

    // TODO very ugly improve and avoid magic numbers
    pub fn calc_score(&self, matched_prediction: &BoundingBox) -> f32 {
        if self.bounding_box.rect.is_empty() {
            return 0.0;
        }
        if self.bounding_box.class != matched_prediction.class {
            return 0.0;
        }
        let mut score = 0.0;
        score += if self
            .possible_location
            .contains(matched_prediction.rect.get_center())
        {
            0.15
        } else {
            0.0
        };
        score += if self
            .possible_location
            .contains(matched_prediction.rect.tl())
        {
            0.1
        } else {
            0.0
        };
        score += if self
            .possible_location
            .contains(matched_prediction.rect.br())
        {
            0.1
        } else {
            0.0
        };

        score += matched_prediction
            .rect
            .iou_percentage(&self.possible_location)
            * 0.15;

        let distance =
            (self.possible_location.width.pow(2) + self.possible_location.height.pow(2)) as f32;
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
            "id: {}\ntype: {:?}\nbox: {}",
            self.id, self.bounding_box.class, self.bounding_box.rect
        )
    }
}

fn apply_deadband(value: f32, deadband: f32) -> f32 {
    if value.abs() < deadband {
        0.0
    } else {
        value
    }
}
