from ultralytics import YOLO
import numpy as np
import cv2
    
def YoloToPointsAndTypes(YOLO_result):
    boxes = YOLO_result.boxes.cpu().numpy()      
    points = np.array(boxes.xyxy.flatten(), dtype=np.uint16)
    types = np.array(boxes.cls.flatten(), dtype=np.uint16)
    return (points, types, len(types))

def frameToArray(coloredframe: np.ndarray):
    return np.array(coloredframe.flatten(), dtype=np.uint8)

def predict(model: YOLO, cap: cv2.VideoCapture):
    # TODO understand when there are mulitpule results and how to handle them. (len(result) > 1)
    success, frame = cap.read()
    if success:    
        frame = prepFrame(frame)
        result = model(frame, verbose=False)
    else: 
        raise ValueError("Frame can not be opened")
    
    return result[0], frame

def prepFrame(frame):
    # TODO make more sofisticated to ask for the field codrs in a window.
    return frame[180 : 480]   
    
    
        
        