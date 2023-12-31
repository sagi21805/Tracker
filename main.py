from ultralytics import YOLO
import cv2
import numpy as np
from time import time, sleep
from python_utils.tracker import Tracker
import keyboard
#TEST CODE DONT TAKE SERIOUSLY


print("loading")
cap = cv2.VideoCapture('/home/sagi21805/dcmp4.mp4')
modelBumpers = YOLO('bumperWeights.pt')
# modelBumpers.to('cuda')
running = True

success, frame = cap.read()

def prepFrame(frame):
    # frame = cv2.resize(frame, (640, 640))
    frame = frame[180 : 480]
    # frame = cv2.resize(frame, (1280, 500))
    return frame

def YoloToPointsAndTypes(YOLO_result):
        boxes = YOLO_result.boxes.cpu().numpy()      
        points = np.array(boxes.xyxy, dtype=np.uint16)
        types = np.array(boxes.cls, dtype=np.uint16)
        return (points, types, len(types))

if success:
    
    frame = prepFrame(frame)
    resultsB = modelBumpers(frame, verbose=False)
    print("[INFO]: Creating Tracker")
    tracker = Tracker(resultsB[0], frame) 
    print("[INFO]: Tracker Created")

def toggle_running():
    global running
    running = not running

keyboard.add_hotkey('space', toggle_running)

while cap.isOpened():
    if running:
        success, frame = cap.read()
        
        if success:
            frame = prepFrame(frame)
            resultsB = modelBumpers(frame, verbose=False)
            tracker.track(resultsB[0], frame)
            

    