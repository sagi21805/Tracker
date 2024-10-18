from typing import List, Optional
from ultralytics import YOLO
import supervision as sv
from .visualizer import visualizer
import numpy as np
from time import time
from tracker.tracker import DynamicEntityTracker, StaticEntityTracker

class GeneralPurposeTracker:

    def __init__(
            self, 
            model_weights: str, 
            video: str, 
            num_of_classes: int,
            pallate: sv.ColorPalette,
            entities_per_class: Optional[List[int]] = None,
            config_path: str = "config.json", 
            skip_frame: int = 0,
            annotate_possible_location = True
        ) -> None:


        self.tracker = (
            DynamicEntityTracker(config_path) if entities_per_class is None 
            else StaticEntityTracker(config_path)
        )
        self.frame_generator = sv.get_video_frames_generator(source_path=video, stride=skip_frame+1)  
        self.model = YOLO(model_weights)

        if annotate_possible_location and len(pallate.colors) != num_of_classes+1:
            raise ValueError(
                f"Provided {len(pallate.colors)} Colors, and "
                f"{num_of_classes + 1} are needed, please notice to give "
                "an extra color for the predicted location box"
            )

        if not annotate_possible_location and len(pallate.colors) != num_of_classes:
            raise ValueError(
                f"Provided {len(pallate.colors)} Colors, and "
                f"{num_of_classes} are needed, please give a color for each class"
            )

        self.visualizer = visualizer(pallate, num_of_classes, annotate_possible_location)
    
    def track(self):
        t = time()
        result, frame = self.predict()
        print(f"prediction time: {time() - t}")
        self.visualizer.annotated_frame = frame
        points, types, confidence = GeneralPurposeTracker.prediction_to_points(result)
        t = time()
        result = self.tracker.track(points, types, confidence)
        print(f"track time: {time() - t}")

        self.visualizer.annotate_detections(result)
        self.visualizer.show_frame(1)

    def process_frame(frame: np.ndarray):
        # Add logic here if needed
        return frame

    def predict(self):
        frame = self.frame_generator.__next__()
        frame = self.process_frame(frame)
        return self.model(frame, verbose = False)[0], frame
    
    @staticmethod
    def prediction_to_points(result):
        boxes = result.boxes.cpu().numpy()      
        return (boxes.xyxy.flatten().astype(np.int32),
                boxes.cls.flatten().astype(np.uint16),
                boxes.conf.flatten().astype(np.float32))

        