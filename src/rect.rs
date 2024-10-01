use std::cmp::{min, max};
use crate::constants::*;

#[repr(C)]
#[derive(Clone)]
pub struct Rect {
    pub(crate) x: i32,
    pub(crate) y: i32,
    pub(crate) width: i32,
    pub(crate) height: i32,
}

impl Rect {

    pub fn new(a: Point, b: Point) -> Self {
        let x = min(a.x, b.x);
        let y = min(a.y, b.y);
        Rect {
            x,  
            y,
            width: max(a.x, b.x) - x,
            height: max(a.y, b.y) - y
        }
    }

    pub fn get_center(&self) -> Point {
        Point::new(self.x + self.width / 2, self.y + self.height / 2)
    }

    pub fn square_distance_to(&self, r: &Rect) -> u32 {
        square_distance(&self.get_center(), &r.get_center())
    }

    pub fn is_close_to(&self, r: &Rect, distance: f64) -> bool {
        self.square_distance_to(r) < distance as u32
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

    pub fn empty(&self) -> bool {
        return self.width <= 0 && self.height <= 0;
    }

    pub fn area(&self) -> i32 {
        self.width * self.height
    }

    pub fn from_points(points: &[i32], size: u16) -> Vec<Rect> {
        let mut rects = Vec::with_capacity(size as usize);
        for p in points.chunks(ELEMENTS_IN_POINT ) {
            let a = Point::new(p[0], p[1]);
            let b = Point::new(p[2], p[3]);
            rects.push(Rect::new(a, b));
        }
        rects
    }
}

impl Default for Rect {

    fn default() -> Self {
        Rect { x: 0, y: 0, width: 0, height: 0 }
    }
}


impl std::fmt::Display for Rect {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:?}\nArea: {}", self.get_center() , self.area())
    }
}

fn square_distance(p1: &Point, p2: &Point) -> u32 {
    let dx = p1.x - p2.x;
    let dy = p1.y - p2.y;
    (dx * dx + dy * dy) as u32
}

