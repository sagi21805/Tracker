use std::cmp::Ordering;
#[repr(C)]
pub struct PointI32 {
    pub x: i32,
    pub y: i32
}

#[repr(C)]
pub struct PointU16 {
    pub x: u16,
    pub y: u16
}


impl PointI32 {

    pub fn distance(&self, p: &PointI32) -> i32 {
        (self.x - p.x) * (self.x - p.x) + (self.y - p.y) * (self.y - p.y)
    }

    pub fn sortByDistance(points: &mut Vec<PointI32>, reference_point: &PointI32) {
        points.sort_by(|a: &PointI32, b: &PointI32| {
            let distance_a = a.distance(reference_point);
            let distance_b = b.distance(reference_point);
            distance_a.partial_cmp(&distance_b).unwrap_or(Ordering::Equal)
        });
    }
}

impl PointU16 {

    pub fn distance(&self, p: &PointU16) -> u16 {
        (self.x - p.x) * (self.x - p.x) + (self.y - p.y) * (self.y - p.y)
    }

    pub fn sortByDistance(points: &mut Vec<PointU16>, reference_point: &PointU16) {
        points.sort_by(|a: &PointU16, b: &PointU16| {
            let distance_a = a.distance(reference_point);
            let distance_b = b.distance(reference_point);
            distance_a.partial_cmp(&distance_b).unwrap_or(Ordering::Equal)
        });
    }
}