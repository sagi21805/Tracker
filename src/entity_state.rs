use crate::bounding_box::BoundingBox;
use crate::constants::*;

#[derive(Clone, Debug)]
pub struct EntityState {
    pub bounding_box: BoundingBox,
    pub vel: Velocity2D,
}

impl EntityState {
    pub fn new(bounding_box: BoundingBox, vel: Velocity2D) -> EntityState {
        EntityState { bounding_box, vel }
    }
}
