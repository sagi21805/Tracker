import python_utils.cpp_utils as cpp
import numpy as np

class Tracker():        
        
    def __init__(self, YOLO_result, coloredframe: np.ndarray) -> None:
        """
        Creates a Tracker object
        -
        """
        setFuncs()
        
        points, types, size = YoloToPointsAndTypes(YOLO_result)

        self.obj = cpp.lib._Tracker(points, types, cpp.c_uint16(size), frameToArray(coloredframe), cpp.c_uint16(coloredframe.shape[0]), cpp.c_uint16(coloredframe.shape[1]))
    
    def track(self, YOLO_result, coloredframe: np.ndarray):
        points, types, size = YoloToPointsAndTypes(YOLO_result)
        cpp.lib._track(self.obj, points, types, cpp.c_uint16(size), frameToArray(coloredframe))
    
def YoloToPointsAndTypes(YOLO_result):
    boxes = YOLO_result.boxes.cpu().numpy()      
    points = np.array(boxes.xyxy.flatten(), dtype=np.uint16)
    types = np.array(boxes.cls.flatten(), dtype=np.uint16)
    return (points, types, len(types))

def frameToArray(coloredframe: np.ndarray):
    return np.array(coloredframe.flatten(), dtype=np.uint8)

def setFuncs():
    cpp.lib._Tracker.argtypes = [cpp.uint16_array, cpp.uint16_array, cpp.uint16, cpp.uint8_array, cpp.uint16, cpp.uint16]
    cpp.lib._Tracker.restype = cpp.object
    cpp.lib._track.argtypes = [cpp.object, cpp.uint16_array, cpp.uint16_array, cpp.uint16, cpp.uint8_array]
        
    
    
        
        