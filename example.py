from source.python.Tracker import Tracker
from pynput import keyboard
import threading

tracker = Tracker("assets/bumperWeights.pt", "assets/TestVideo.mp4") #load weights and a video.
is_running = True

def main_code():
    while True:
        if is_running:
            tracker.track(show_time=True) #Track on a loop and show performence.
            

def on_press(key): #on press commend to stop the video.
    global is_running
    try:
        if key.char == 'p':
            is_running = not is_running 
            print("Background code paused" if not is_running else "Background code resumed")
    except AttributeError:
        pass

listener = keyboard.Listener(on_press=on_press) #set the keyboard listener.

listener_thread = threading.Thread(target=listener.start) # put the listener on a different thread.

if __name__ == "__main__": 
    listener_thread.start() #start the listener thread. 
    main_code() #start the main Tracking code.
