// mod config;
mod bounding_box;
mod constants;
mod entity;
mod entity_state;
mod rect;
mod tracker;
mod utils;
// mod entity_utils;
mod point;

#[cfg(test)]
mod tests {
    use crate::constants::*;
    use crate::rect::Rect;
    #[test]
    fn test_rect() {
        let rect = Rect::from_points(Point::new(2, 2), Point::new(5, 5));
        assert_eq!(9, rect.area());
        assert_eq!(Point::new(3, 3), rect.get_center())
    }

    #[test]
    fn test_point() {
        let p1 = Point::new(3, 4);
        let p2 = Point::new(1, 2);
        assert_eq!(8, p1.square_distance(&p2));
        assert_eq!(Point::new(4, 6), p1 + p2);
        assert_eq!(Point::new(2, 2), p1 - p2);
    }
}
