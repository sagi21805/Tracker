use crate::constants::*;
use std::collections::LinkedList;
use crate::bounding_box::BoundingBox;

pub struct Trajectory {
    bounding_box: BoundingBox,
    vel: Velocity2D 
}

impl Trajectory {

    pub fn new(bounding_box: BoundingBox, vel: Velocity2D) -> Trajectory {
        Trajectory { bounding_box, vel}
    }

}
