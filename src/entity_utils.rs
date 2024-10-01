use opencv::{core, imgproc, prelude::*};
use std::cmp::max;

// Assuming these are defined elsewhere in your Rust project
use crate::{Point, Velocity2D, Rect, BoundingBox, Entity, EntityType};

impl Entity {
    pub fn calc_and_set_velocity(&mut self) {
        if self.trajectory.len() >= _smoothingFrames {
            let start_point = &self.trajectory[_smoothingFrames - 1].box.rect.center;
            let end_point = &self.trajectory[0].box.rect.center;
            let raw_velocity = Velocity2D::new(
                (end_point.x - start_point.x) / _smoothingFrames as f32,
                (end_point.y - start_point.y) / _smoothingFrames as f32,
            );
            self.velocity.x = apply_deadband(raw_velocity.x, _velocityDeadBand);
            self.velocity.y = apply_deadband(raw_velocity.y, _velocityDeadBand);
        }
    }

    pub fn predict_next_bounding_box(&mut self) {
        self.bounding_box.rect.x += self.velocity.x;
        self.bounding_box.rect.y += self.velocity.y;
    }

    pub fn square_distance_to(&self, e: &Entity) -> u32 {
        self.get_bounding_box().rect.square_distance_to(&e.get_bounding_box().rect)
    }

    pub fn square_distance_to_rect(&self, r: &Rect) -> u32 {
        self.get_bounding_box().rect.square_distance_to(r)
    }

    pub fn predict_possible_locations(&mut self) {
        let r = &mut self.get_bounding_box().rect;
        let vx = self.velocity.x * _velocityCoefficient;
        let vy = self.velocity.y * _velocityCoefficient;
        let w = r.width as f32 * _sizeCoefficient * vx.signum() * (self.times_not_found as f32 + 5.0) / 5.0;
        let h = r.height as f32 * _sizeCoefficient * vy.signum() * (self.times_not_found as f32 + 5.0) / 5.0;
        self.possible_location = Rect::new(
            r.center + Point::new(vx as i32 + w as i32, vy as i32 + h as i32),
            r.center - Point::new(w as i32, h as i32),
        );
    }

    pub fn choose_color(&self) -> core::Scalar {
        match self.bounding_box.entity_type {
            EntityType::RedRobot => _colors[&EntityType::RedRobot],
            EntityType::BlueRobot => _colors[&EntityType::BlueRobot],
            _ => core::Scalar::new(0.0, 0.0, 0.0, 0.0),
        }
    }

    pub fn draw(&self, frame: &mut Mat) {
        self.get_bounding_box().rect.draw(frame, self.color);
        imgproc::put_text(
            frame,
            &self.id.to_string(),
            self.bounding_box.rect.tl(),
            imgproc::FONT_HERSHEY_DUPLEX,
            1.0,
            core::Scalar::new(255.0, 255.0, 0.0, 0.0),
            2,
            imgproc::LINE_8,
            false,
        ).unwrap();
        imgproc::put_text(
            frame,
            &format!("{:.2}", self.bounding_box.confidence),
            self.bounding_box.rect.center - Point::new(24, -12),
            imgproc::FONT_HERSHEY_DUPLEX,
            1.0,
            core::Scalar::new(0.0, 255.0, 255.0, 0.0),
            2,
            imgproc::LINE_8,
            false,
        ).unwrap();
    }

    pub fn combine_bounding_box(&mut self, b: &BoundingBox) {
        self.bounding_box = self.bounding_box.merge_bounding_box(b);
    }

    pub fn calc_score(&self, matched_prediction: &BoundingBox) -> f32 {
        if self.bounding_box.rect.is_empty() {
            return 0.0;
        }
        if self.bounding_box.entity_type != matched_prediction.entity_type {
            return 0.0;
        }
        let mut score = 0.0;
        score += if self.get_possible_location().contains(matched_prediction.rect.center) { 0.15 } else { 0.0 };
        score += if self.get_possible_location().contains(matched_prediction.rect.tl()) { 0.1 } else { 0.0 };
        score += if self.get_possible_location().contains(matched_prediction.rect.br()) { 0.1 } else { 0.0 };

        score += matched_prediction.rect.iou_percentage(&self.possible_location) * 0.15;

        let distance = (self.possible_location.width.pow(2) + self.possible_location.height.pow(2)) as f32;
        let slope = (0.75 - 1.0) / distance;
        let current_distance = self.square_distance_to_rect(&matched_prediction.rect) as f32;
        score += f32::max(0.5 * (slope * current_distance + 1.0), 0.0);
        score
    }

    pub fn outside_frame(&self, rows: u16, cols: u16) -> bool {
        // Implementation not provided in the original code
        false
    }
}

impl std::fmt::Display for Entity {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "id: {}\ntype: {:?}\nbox: {}", self.get_id(), self.get_type(), self.get_bounding_box().rect)
    }
}

fn apply_deadband(value: f32, deadband: f32) -> f32 {
    if value.abs() < deadband {
        0.0
    } else {
        value
    }
}

