from tracker import _Tracker
from ultralytics import YOLO
import cv2
import supervision as sv
from src.python.visualization import visualizer
import numpy as np
from time import time


class Tracker:

    def __init__(
            self, 
            model_weights: str, 
            video: str, 
            config_path: str = "config.json", 
            skip_frame: int = 2
        ) -> None:
        self.tracker = _Tracker(config_path) 
        self.frame_generator = sv.get_video_frames_generator(source_path=video, stride=skip_frame)  
        self.model = YOLO(model_weights)
        self.visualizer = visualizer(sv.ColorPalette([sv.Color.BLUE, sv.Color.RED]))
    def track(self):
        t = time()
        result, frame = self.predict()
        print(f"prediction time: {time() - t}")
        self.visualizer.annotated_frame = frame
        points, types, confidence = Tracker.prediction_to_points(result)
        t = time()
        result = self.tracker.track(points, types, confidence)
        print(f"track time: {time() - t}")

        self.visualizer.annotate_detections(result)
        self.visualizer.show_frame(1)

    def process_frame(frame: np.ndarray):
        return frame[0:460]

    def predict(self):
        frame = self.frame_generator.__next__()
        frame = Tracker.process_frame(frame)
        return self.model(frame, verbose = False)[0], frame
    
    @staticmethod
    def prediction_to_points(result):
        boxes = result.boxes.cpu().numpy()      
        return (boxes.xyxy.flatten().astype(np.int32),
                boxes.cls.flatten().astype(np.uint16),
                boxes.conf.flatten().astype(np.float32))

        