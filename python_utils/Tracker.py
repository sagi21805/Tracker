import python_utils.cpp_utils as cpp
from ultralytics import YOLO
import cv2
from python_utils.tracker_utils import *

class Tracker():        
    
    @staticmethod
    def setFuncs():
        cpp.lib._Tracker.argtypes = [cpp.uint16_array, cpp.uint16_array, cpp.uint16, cpp.uint8_array, cpp.uint16, cpp.uint16]
        cpp.lib._Tracker.restype = cpp.object
        cpp.lib._track.argtypes = [cpp.object, cpp.uint16_array, cpp.uint16_array, cpp.uint16, cpp.uint8_array]
    
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
            on_startup(frame)
        else: 
            raise ValueError("Frame can not be opened")
        
        result, frame = predict(self.model, self.cap)
        Tracker.setFuncs()
        points, types, size = YoloToPointsAndTypes(result)
        self.Tracker = cpp.lib._Tracker(points, types, cpp.c_uint16(size), frameToArray(frame), cpp.c_uint16(frame.shape[0]), cpp.c_uint16(frame.shape[1]))
    
    def track(self):
        result, frame = predict(self.model, self.cap)
        points, types, size = YoloToPointsAndTypes(result)
        cpp.lib._track(self.Tracker, points, types, cpp.c_uint16(size), frameToArray(frame))