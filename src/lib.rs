use pyo3::prelude::*;
mod bounding_box;
mod constants;
mod entity;
mod entity_state;
mod point;
mod rect;
mod tracking;
mod utils;
mod config;

#[pymodule]
fn tracker(py: Python, m: &Bound<'_, PyModule>) -> PyResult<()> {
    m.add_class::<tracking::_Tracker>()?;
    Ok(())
}

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
