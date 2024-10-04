use crate::point::Point as P;

pub const ELEMENTS_IN_POINT: usize = 4;
pub const VEL_DEAD_BAND: u8 = 5;
pub const MIN_SCORE: f32 = 0.7;
pub const MOVE_TO_LAST_SEEN: u64 = 10;
pub const MIN_CONFIDENCE: f32 = 0.0;
pub const SIZE_COEFFICIENT: f32 = 0.75;
pub const VEL_COEFFICIENT: f32 = 3.0;
pub const TO_VISUALIZE: bool = true;
pub const WAIT_KEY: u8 = 1;
pub const BLUE: [u8; 3] = [0, 0, 255];
pub const RED: [u8; 3] = [255, 0, 0];
pub const RATIO: f32 = 0.8;

pub type Point = P<i32>;
pub type Velocity2D = P<i32>;
