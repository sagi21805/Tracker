from ultralytics import YOLO
import cv2
import numpy as np
from time import time, sleep
from python_utils.tracker import Tracker
from pynput import keyboard
import threading
#TEST CODE DONT TAKE SERIOUSLY


print("loading")
cap = cv2.VideoCapture('/home/sagi21805/dcmp4.mp4')
modelBumpers = YOLO('bumperWeights.pt')
print("done loading")
is_running = True
success, frame = cap.read()

def prepFrame(frame):
    return frame[180 : 480]

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



def background_code():
    while cap.isOpened():
        if is_running:
            success, frame = cap.read()
            if success: 
                frame = prepFrame(frame)
                resultsB = modelBumpers(frame, verbose=False)
                tracker.track(resultsB[0], frame)
            

# Function to be called when a key is pressed
def on_press(key):
    global is_running
    try:
        if key.char == 'p':
            is_running = not is_running  # Toggle the state
            print("Background code paused" if not is_running else "Background code resumed")
    except AttributeError:
        pass

# Start the background thread
background_thread = threading.Thread(target=background_code)
background_thread.start()

# Set up the listener for key presses
with keyboard.Listener(on_press=on_press) as listener:
    listener.join() 

