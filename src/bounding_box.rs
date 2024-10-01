use std::cmp::{min, max};
use crate::constants::*;
use crate::rect::Rect;


#[derive(Clone)]
pub struct BoundingBox {
    pub(crate) class: u16,
    pub(crate) rect: Rect,
    pub(crate) confidence: f32,
}

impl BoundingBox {

    fn combine_bounding_box(&self, b: &BoundingBox) -> BoundingBox {
        todo!()
    //     if self.is_empty() {
    //         return b.clone();
    //     }
    //     if b.is_empty() {
    //         return self.clone();
    //     }
    //     BoundingBox {
    //         rect: Rect {
    //             x: min(self.rect.x, b.rect.x),
    //             y: min(self.rect.y, b.rect.y),
    //             width: max(self.rect.width + self.rect.x, b.rect.width + b.rect.x) - min(self.rect.x, b.rect.x),
    //             height: max(self.rect.height + self.rect.y, b.rect.height + b.rect.y) - min(self.rect.y, b.rect.y),
    //         },
    //         class: b.class,
    //         confidence: (self.confidence + b.confidence) / 2.0,
    //     }
    }

    fn merge_bounding_box(&self, b: &BoundingBox) -> BoundingBox {
    //     if self.is_empty() {
    //         return b.clone();
    //     }
    //     if b.is_empty() {
    //         return self.clone();
    //     }
    //     let x = (self.rect.x + b.rect.x) / 2;
    //     let y = (self.rect.y + b.rect.y) / 2;
    //     let w = (self.rect.width + b.rect.width) / 2;
    //     let h = (self.rect.height + b.rect.height) / 2;
    //     BoundingBox {
    //         rect: Rect { x, y, width: w, height: h },
    //         class: b.class,
    //         confidence: (self.confidence + b.confidence) / 2.0,
    //     }
        todo!()
    }

    fn is_empty(&self) -> bool {
        self.rect.empty()
    }
}

fn generate_bounding_boxes(points: &[i32], types: &[u16], confidences: &[f32], size: usize) -> Vec<BoundingBox> {
    // let mut bounding_boxes = Vec::with_capacity(size);
    // for i in 0..size {
    //     let current_point = &points[i * ELEMENTS_IN_POINT as usize..];
    //     let current_type = types[i];
    //     let current_confidence = confidences[i];
    //     if current_confidence > MIN_CONFIDENCE {
    //         bounding_boxes.push(BoundingBox {
    //             rect: Rect {
    //                 x: current_point[0],
    //                 y: current_point[1],
    //                 width: current_point[2],
    //                 height: current_point[3],
    //             },
    //             class: current_type,
    //             confidence: current_confidence,
    //         });
    //     }
    // }
    // bounding_boxes
    todo!()
}

