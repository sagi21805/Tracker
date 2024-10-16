import cv2
import numpy as np
import supervision as sv
from tracker.tracker import Entity
class visualizer():

    def __init__(
            self, 
            pallate: sv.ColorPalette,
            num_of_classes: int,
            annotate_possible_location: bool
            ) -> None:
        
        self.annotated_frame = np.zeros(0)
        self.box_annotator = sv.BoxAnnotator(pallate)
        self.num_of_classes = num_of_classes
        self.annotate_possible_location = annotate_possible_location
        self.label_annotator = sv.LabelAnnotator(
            color = pallate,
            text_position=sv.Position.TOP_CENTER,
            text_thickness=2,
            text_scale=0.6,
            text_padding=5
        )

    def annotate_detections(self, tracker_result: list[Entity]):
        xyxy = np.array([np.array(entity.bounding_box.rect.into_xyxy()) for entity in tracker_result])
        labels = np.array([f"id: {entity.id}, conf: {round(entity.bounding_box.confidence, 2)}" for entity in tracker_result])
        class_id = np.array([np.array(entity.bounding_box.group_id) for entity in tracker_result])
        detections = sv.Detections(xyxy=xyxy, class_id=class_id, tracker_id=labels)

        if self.annotate_possible_location:
            locations = np.array([np.array(entity.predicted_location.into_xyxy()) for entity in tracker_result])
            locations = sv.Detections(xyxy=locations, class_id=np.array([self.num_of_classes for _ in range(len(xyxy))]))
            self.annotated_frame = self.box_annotator.annotate(
                scene=self.annotated_frame, detections=locations
        )
            
        self.annotated_frame = self.box_annotator.annotate(
            scene=self.annotated_frame, detections=detections
        )
        self.annotated_frame = self.label_annotator.annotate(
            scene=self.annotated_frame, detections=detections, labels=labels
        )
    

    def show_frame(self, wait_time: int):
        cv2.imshow("Tracking", self.annotated_frame)
        cv2.waitKey(wait_time)

    def type_color(type: int):
        match type:
            case 0:
                return (255, 0, 0)
            case 1: 
                return (0, 0, 255)

