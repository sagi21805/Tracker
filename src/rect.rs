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

    pub fn iou_percentage(&self, other: &Rect) -> f32 {
        let x_overlap = min(self.x, other.x) - max(self.x, other.x);
        let y_overlap = min(self.y, other.y) - max(self.y, other.y);

        if x_overlap <= 0 || y_overlap <= 0 {
            return 0.0;
        }

        let intersection_area = x_overlap * y_overlap;
        intersection_area as f32 / self.area() as f32
    }

    pub fn is_empty(&self) -> bool {
        return self.width <= 0 && self.height <= 0;
    }

    pub fn area(&self) -> i32 {
        self.width * self.height
    }

    pub fn vec_from_points(points: &[i32], size: u16) -> Vec<Rect> {
        let mut rects = Vec::with_capacity(size as usize);
        for p in points.chunks(ELEMENTS_IN_POINT) {
            let a = Point::new(p[0], p[1]);
            let b = Point::new(p[2], p[3]);
            rects.push(Rect::from_points(a, b));
        }
        rects
    }

    #[inline]
    pub fn contains(&self, p: Point) -> bool {
        p.x >= self.x && p.x <= self.x + self.width && p.y >= self.y && p.y <= self.y + self.height
    }

    #[inline]
    pub fn tl(&self) -> Point {
        Point::new(self.x, self.y)
    }

    #[inline]
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
