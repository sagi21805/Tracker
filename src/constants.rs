use crate::point::Point as P;

pub const ELEMENTS_IN_POINT: usize = 4;
pub const MIN_SCORE: f32 = 0.0;
pub const MOVE_TO_LAST_SEEN: u64 = 10;
pub const MIN_CONFIDENCE: f32 = 0.0;
pub const SIZE_COEFFICIENT: f32 = 0.75;
pub const VEL_COEFFICIENT: f32 = 3.0;
pub const RATIO: f32 = 0.01;

pub type Point = P<i32>;
pub type Velocity2D = P<i32>;
