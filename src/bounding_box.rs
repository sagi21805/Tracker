use crate::rect::Rect;
use crate::utils;
use std::cmp::{max, min};
use pyo3::prelude::*;

#[pyclass]
#[derive(Clone)]
pub struct BoundingBox {
    pub(crate) class: u16,
    pub(crate) rect: Rect,
    pub(crate) confidence: f32,
}

#[pymethods]
impl BoundingBox {

    #[new]
    pub fn new(class: u16, rect: Rect, confidence: f32) -> Self{

        BoundingBox {
            class, rect, confidence
        }

    }
}

impl BoundingBox {
    pub fn combine(&mut self, other: &BoundingBox) {
        // TODO see if neceserry
        // todo!()
        // if self.is_empty() {
        //     return b.clone();
        // }
        // if b.is_empty() {
        //     return self.clone();
        // }
        self.rect = Rect {
            x: min(self.rect.x, other.rect.x),
            y: min(self.rect.y, other.rect.y),
            width: max(self.rect.width, other.rect.width),
            height: max(self.rect.height, other.rect.height),
        };

        self.confidence = (self.confidence + other.confidence) / 2.0;

    }

    pub fn merge(&mut self, other: &BoundingBox, ratio: f32) {
        //     if self.is_empty() {
        //         return b.clone();
        //     }
        //     if b.is_empty() {
        //         return self.clone();
        //     }
        self.rect = Rect {
            x: utils::interpolate(self.rect.x, other.rect.x, ratio),
            y: utils::interpolate(self.rect.y, other.rect.y, ratio),
            width: utils::interpolate(self.rect.width, other.rect.width, ratio),
            height: utils::interpolate(self.rect.height, other.rect.height, ratio),
        };

        self.confidence = utils::interpolate(self.confidence, other.confidence, ratio);
        // todo!()
    }

    pub fn is_empty(&self) -> bool {
        self.rect.is_empty()
    }
}

impl std::fmt::Display for BoundingBox {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "Rect: {}, class: {}, confidence: {}", self.rect, self.class, self.confidence)
    }
}
