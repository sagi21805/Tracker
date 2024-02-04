import source.python.c_utils as c
from ultralytics import YOLO
import cv2
from source.python.tracker_utils import *
from time import time

class Tracker():        
    
    @staticmethod
    def setFuncs():
        c.lib._Tracker.argtypes = [c.int32_array, c.uint16_array, c.float32_array,c.uint16, c.uint8_array, c.uint16, c.uint16]
        c.lib._Tracker.restype = c.object
        c.lib._track.argtypes = [c.object, c.int32_array, c.uint16_array, c.float32_array, c.uint16, c.uint8_array]
    
    def __init__(self, model_path: str, video) -> None:
        """Creates A Tracker Object
        -
        `Args:`
            - `model_path (str):` The Path To The Model Weights (.pt file)
            - `video (int or str):` The Path To The Tracked video *OR* The Index of the Camera.
        """
        print("[INFO]: Loading Model")
        self.model = YOLO(model_path)
        print("[INFO]: Done Loading Model")
        print("[INFO]: Loading video")
        self.cap = cv2.VideoCapture(video)
        print("[INFO]: Done Loading Video")
        
        success, frame = self.cap.read()
        if success:
            pass
            # on_startup(frame)
        else: 
            raise ValueError("Frame can not be opened")
        
        result, frame = predict(self.model, self.cap)
        Tracker.setFuncs()
        points, types, confidence, size = YoloToPointsAndTypes(result)
        self.Tracker = c.lib._Tracker(points, types, confidence, c.c_uint16(size), frameToArray(frame),\
                                      c.c_uint16(frame.shape[0]), c.c_uint16(frame.shape[1]))
        
    def track(self, show_time = False):
        model_time = time()
        result, frame = predict(self.model, self.cap)
        model_time = time() - model_time
        points, types, confidence, size = YoloToPointsAndTypes(result)
        track_time = time()
        c.lib._track(self.Tracker, points, types, confidence, c.c_uint16(size), frameToArray(frame))
        track_time = time() - track_time
        if show_time:
            print(f"Model Time: {model_time}, Tracking Time: {track_time}")