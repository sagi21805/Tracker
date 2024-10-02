use crate::rect::Rect;
use crate::utils;
use std::cmp::{max, min};

#[derive(Clone)]
pub struct BoundingBox {
    pub(crate) class: u16,
    pub(crate) rect: Rect,
    pub(crate) confidence: f32,
}

impl BoundingBox {
    pub fn combine(&self, other: &BoundingBox) -> BoundingBox {
        // TODO see if neceserry
        // todo!()
        // if self.is_empty() {
        //     return b.clone();
        // }
        // if b.is_empty() {
        //     return self.clone();
        // }
        BoundingBox {
            rect: Rect {
                x: min(self.rect.x, other.rect.x),
                y: min(self.rect.y, other.rect.y),
                width: max(self.rect.width, other.rect.width),
                height: max(self.rect.height, other.rect.height),
            },
            class: self.class,
            confidence: (self.confidence + other.confidence) / 2.0,
        }
    }

    pub fn merge(&self, other: &BoundingBox, ratio: f32) -> BoundingBox {
        //     if self.is_empty() {
        //         return b.clone();
        //     }
        //     if b.is_empty() {
        //         return self.clone();
        //     }
        BoundingBox {
            rect: Rect {
                x: utils::interpolate(self.rect.x, other.rect.x, ratio),
                y: utils::interpolate(self.rect.y, other.rect.y, ratio),
                width: utils::interpolate(self.rect.width, other.rect.width, ratio),
                height: utils::interpolate(self.rect.height, other.rect.height, ratio),
            },
            class: self.class,
            confidence: utils::interpolate(self.confidence, other.confidence, ratio),
        }
        // todo!()
    }

    pub fn is_empty(&self) -> bool {
        self.rect.is_empty()
    }
}
