from ultralytics import YOLO
import numpy as np
import cv2

Ymin = 0
Ymax = 0
Y_coordinates = []

def YoloToPointsAndTypes(YOLO_result):
    boxes = YOLO_result.boxes.cpu().numpy()      
    points = np.array(boxes.xyxy.flatten(), dtype=np.uint16)
    types = np.array(boxes.cls.flatten(), dtype=np.uint16)
    return (points, types, len(types))

def frameToArray(coloredframe: np.ndarray):
    return np.array(coloredframe.flatten(), dtype=np.uint8)

def predict(model: YOLO, cap: cv2.VideoCapture):
    success, frame = cap.read()
    if success:    
        frame = prepFrame(frame)
        result = model(frame, verbose=False)
    else: 
        raise ValueError("Frame can not be opened")
    
    return result[0], frame

def on_startup(frame):
    global Ymin, Ymax, Y_coordinates
    cv2.imshow('Select Y Coordinates To Cut The Screen', frame)
    cv2.setMouseCallback('Select Y Coordinates To Cut The Screen',get_coordinates)
    captured_clicks = 2
    while len(Y_coordinates) < captured_clicks:
        cv2.waitKey(1)
    cv2.destroyAllWindows()
    Ymin = min(Y_coordinates)
    Ymax = max(Y_coordinates)

def get_coordinates(event, x, y, flags, param):
    global Y_coordinates
    if event == cv2.EVENT_LBUTTONDOWN:
        Y_coordinates.append(y)
        print(f"Left button clicked at ({x}, {y})")


def prepFrame(frame):
    global Ymin, Ymax
    return frame[Ymin: Ymax]   
    
    
        
        