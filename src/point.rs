use num::Zero;
use std::ops::{Add, Mul, Sub};

#[derive(Debug, Copy, Clone, PartialEq)]
pub struct Point<T> {
    pub x: T,
    pub y: T,
}

impl<T> Point<T>
where
    T: Add<Output = T> + Sub<Output = T> + Mul<Output = T> + Copy,
{
    // Create a new point
    pub fn new(x: T, y: T) -> Self {
        Self { x, y }
    }

    pub fn square_distance(&self, other: &Point<T>) -> T {
        (self.x - other.x) * (self.x - other.x) + (self.y - other.y) * (self.y - other.y)
    }
}

// Implementing the Add trait for Point<T>
impl<T> Add for Point<T>
where
    T: Add<Output = T> + Copy,
{
    type Output = Point<T>;

    fn add(self, other: Point<T>) -> Point<T> {
        Point {
            x: self.x + other.x,
            y: self.y + other.y,
        }
    }
}

// Implementing the Sub trait for Point<T>
impl<T> Sub for Point<T>
where
    T: Sub<Output = T> + Copy,
{
    type Output = Point<T>;

    fn sub(self, other: Point<T>) -> Point<T> {
        Point {
            x: self.x - other.x,
            y: self.y - other.y,
        }
    }
}

// Implementing the Mul trait for scalar multiplication
impl<T> Mul<T> for Point<T>
where
    T: Mul<Output = T> + Copy,
{
    type Output = Point<T>;

    fn mul(self, factor: T) -> Point<T> {
        Point {
            x: self.x * factor,
            y: self.y * factor,
        }
    }
}

// Implementing the Default trait for Point<T>
impl<T> Default for Point<T>
where
    T: Zero,
{
    fn default() -> Self {
        Point {
            x: T::zero(),
            y: T::zero(),
        }
    }
}
