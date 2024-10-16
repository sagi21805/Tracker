use crate::rect::Rect;
use crate::utils;
use pyo3::prelude::*;
use std::cmp::{max, min};

#[pyclass]
#[derive(Clone)]
pub struct BoundingBox {
    #[pyo3(get)]
    pub(crate) group_id: u16,

    #[pyo3(get)]
    pub(crate) rect: Rect,

    #[pyo3(get)]
    pub(crate) confidence: f32,
}

#[pymethods]
impl BoundingBox {
    #[new]
    pub fn new(group_id: u16, rect: Rect, confidence: f32) -> Self {
        BoundingBox {
            group_id,
            rect,
            confidence,
        }
    }
}

impl BoundingBox {
    pub fn combine(&mut self, other: &BoundingBox) {
        self.rect = Rect {
            x: min(self.rect.x, other.rect.x),
            y: min(self.rect.y, other.rect.y),
            width: max(self.rect.width, other.rect.width),
            height: max(self.rect.height, other.rect.height),
        };

        self.confidence = (self.confidence + other.confidence) / 2.0;
    }

    pub fn merge(&mut self, other: &BoundingBox, ratio: f32) {
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
        write!(
            f,
            "Rect: {}, type: {}, confidence: {}",
            self.rect, self.group_id, self.confidence
        )
    }
}
