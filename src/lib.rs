// mod config;
mod bounding_box;
mod entity;
mod rect;
// mod tracker;
// mod utils;
mod constants;
mod trajectory;
// mod entity_utils;

pub fn add(left: u64, right: u64) -> u64 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::rect::Rect;
    use crate::constants::*;
    #[test]
    fn it_works() {
        let rect = Rect::new(Point::new(2, 2), Point::new(5, 5));
        println!("{}", rect);
        println!("center: {:?}", rect.get_center());
    }


}
