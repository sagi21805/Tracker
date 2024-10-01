use std::sync::atomic::{AtomicU16, Ordering};
use std::rc::Rc;
use std::cell::RefCell;

// Assuming these are defined elsewhere or imported
use crate::constants::*;
use crate::bounding_box::BoundingBox;
use std::collections::LinkedList;
use crate::rect::Rect;
use crate::trajectory::Trajectory;

pub struct Entity {
    id: u16,
    bounding_box: BoundingBox,
    velocity: Velocity2D,
    trajectory: LinkedList<Trajectory>,
    possible_location: Rect,
    color: [u8; 3], // Assuming color is represented as RGB
}

impl Entity {

    pub fn new(bounding_box: BoundingBox) -> Self {

        static STARTING_ID: AtomicU16 = AtomicU16::new(0);
        let id = STARTING_ID.fetch_add(1, Ordering::SeqCst);
        
        let mut entity = Entity {
            id,
            bounding_box,
            velocity: Velocity2D::new(0.0, 0.0),
            trajectory: LinkedList::new(),
            possible_location: Rect::default(),
            color: Self::choose_color(),
        };
        
        entity.trajectory.push_front(
            Trajectory::new(
                entity.bounding_box.clone(), Velocity2D::new(0.0, 0.0)
            )
        );        
        return entity;
    }
    
    pub fn set_bounding_box(&mut self, mut bounding_box: BoundingBox) {
        if self.trajectory.len() >= 2 {
            let v = 0.8; //percentage of current bounding box and last bounding box
            //move into a differenct function
            let mut x = bounding_box.rect.x as f32 * v;
            let mut y = bounding_box.rect.y as f32 * v;
            let mut width = bounding_box.rect.width as f32 * v;
            let mut height = bounding_box.rect.height as f32 * v;
            
            if let Some(first) = self.trajectory.iter().nth(0) {
                let v = 1.0 - v;
                x += first.bounding_box.rect.x * v;
                y += first.bounding_box.rect.y * v;
                width += first.bounding_box.rect.width * v;
                height += first.bounding_box.rect.height * v;
            }
            
            self.bounding_box = BoundingBox::new(
                Rect::new(x, y, width, height),
                self.get_type(),
                bounding_box.confidence
            );
        } else {
            self.bounding_box = bounding_box;
        }
    }
    
    pub fn add_to_trajectory(&mut self, t: Trajectory) {
        self.trajectory.push_front(t);
    }
    
    
    fn choose_color() -> [u8; 3] {
        // Implement color choosing logic here
        [0, 0, 0]
    }
    
    fn predict_possible_locations(&mut self) {
        // Implement prediction logic here
    }
}

