use pyo3::{pyclass, pymethods};

use crate::constants::*;
use std::cmp::{max, min};

#[pyclass]
#[derive(Clone)]
pub struct Rect {
    #[pyo3(get)]
    pub(crate) x: i32,

    #[pyo3(get)]
    pub(crate) y: i32,

    #[pyo3(get)]
    pub(crate) width: i32,

    #[pyo3(get)]
    pub(crate) height: i32,
}

#[pymethods]
impl Rect {
    #[new]
    pub fn new(x: i32, y: i32, width: i32, height: i32) -> Self {
        Rect {
            x,
            y,
            width,
            height,
        }
    }

    pub fn into_xyxy(&self) -> Vec<i32> {
        return vec![self.x, self.y, self.x + self.width, self.y + self.height]
    }
}

impl Rect {
    pub fn from_points(a: Point, b: Point) -> Self {
        let x = min(a.x, b.x);
        let y = min(a.y, b.y);
        Rect {
            x,
            y,
            width: max(a.x, b.x) - x,
            height: max(a.y, b.y) - y,
        }
    }

    pub fn get_center(&self) -> Point {
        Point::new(self.x + self.width / 2, self.y + self.height / 2)
    }

    pub fn square_distance_to(&self, r: &Rect) -> i32 {
        self.get_center().square_distance(&r.get_center())
    }

//   pub fn iou_percentage(&self, other: &Rect) -> f32 {
        
//         if self.contains(other.tl()) && self.contains(other.br()) {
//             return 1.0
//         }
        
//         let x1 = max(self.x, other.x);
//         let y1 = max(self.y, other.y);
//         let x2 = min(self.x + self.width, other.x + other.width);
//         let y2 = min(self.y + self.height, other.y + other.height);

//         let x_overlap = x2 - x1;
//         let y_overlap = y2 - y1;

//         if x_overlap <= 0 || y_overlap <= 0 {
//             return 0.0;
//         }

//         let intersection_area = x_overlap * y_overlap;
//         let union_area = self.area() + other.area() - intersection_area;
        
//         intersection_area as f32 / union_area as f32
//     }

    pub fn percentage_inside(&self, small_rect: &Rect) -> f32 {
        // Find the coordinates of the intersection
        let x1_inter = self.x.max(small_rect.x);
        let y1_inter = self.y.max(small_rect.y);
        let x2_inter = (self.x + self.width).min(small_rect.x + small_rect.width);
        let y2_inter = (self.y + self.height).min(small_rect.y + small_rect.height);

        // Check if there is an intersection
        if x1_inter < x2_inter && y1_inter < y2_inter {
            // Calculate the area of the intersection
            let intersection_area = (x2_inter - x1_inter) * (y2_inter - y1_inter);

            // Calculate the percentage of the small rectangle inside the big rectangle
            if small_rect.area() > 0 {
                return intersection_area as f32 / small_rect.area() as f32;
            }
        }

        // If no intersection, return 0
        0.0
    }

    pub fn is_empty(&self) -> bool {
        return self.width <= 0 && self.height <= 0;
    }

    pub fn area(&self) -> i32 {
        self.width * self.height
    }

    pub fn contains(&self, p: Point) -> bool {
        p.x >= self.x && p.x <= self.x + self.width && p.y >= self.y && p.y <= self.y + self.height
    }

    pub fn tl(&self) -> Point {
        Point::new(self.x, self.y)
    }

    pub fn br(&self) -> Point {
        Point::new(self.x + self.width, self.y + self.height)
    }

}

impl Default for Rect {
    fn default() -> Self {
        Rect {
            x: 0,
            y: 0,
            width: 0,
            height: 0,
        }
    }
}

impl std::fmt::Display for Rect {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:?}\nArea: {}", self.get_center(), self.area())
    }
}
