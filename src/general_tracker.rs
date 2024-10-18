use crate::config::{self, Config};
use crate::entity::Entity;
use crate::entity_state::EntityState;
use crate::point::Point;
use crate::rect::Rect;
use crate::bounding_box::BoundingBox;
use numpy::*;
use pyo3::pymethods;
use pyo3::prelude::*;
// pub struct GeneralTracker {
//     pub(crate) entities: Vec<Entity>,
//     pub(crate) config: Config,
// }

pub trait TrackerUtils {
    
    fn get_config(&self) -> &Config;

    fn generate_boxes(
        &self,
        points: PyReadonlyArray1<i32>,
        typees: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
    ) -> Vec<BoundingBox> {
        let mut bounding_boxes = Vec::with_capacity(points.len());
        let points_slice = points.as_slice().expect("array is not contigous");
        let confidences_slice = confidences.as_slice().expect("array is not contigous");
        let typees_slice = typees.as_slice().expect("array is not contigous");
        let config = self.get_config();

        for ((point, confidence), group_id) in points_slice
            .chunks(config.elements_in_point)
            .zip(confidences_slice.iter())
            .zip(typees_slice.iter())
        {
            if *confidence > config.min_confidence {
                bounding_boxes.push(BoundingBox {
                    rect: Rect::from_points(
                        Point::new(point[0], point[1]),
                        Point::new(point[2], point[3]),
                    ),
                    group_id: *group_id,
                    confidence: *confidence,
                });
            }
        }
        return bounding_boxes;
    }

    fn match_entity(&mut self, recognition: &mut Vec<BoundingBox>);

    fn start_cycle(&mut self);
 
    fn manage_entities(&mut self);
}

pub trait GeneralTracker {

    fn new(config_path: &str) -> Self;

    fn track_entities(
        &mut self,
        points: PyReadonlyArray1<i32>,
        typees: PyReadonlyArray1<u16>,
        confidences: PyReadonlyArray1<f32>,
    ) -> Vec<Entity>;
}



