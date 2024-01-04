from python_utils.Tracker import Tracker
from pynput import keyboard
import threading
#TEST CODE DONT TAKE SERIOUSLY


tracker = Tracker("bumperWeights.pt", "/home/sagi21805/dcmp4.mp4")
is_running = True

def background_code():
    while True:
        if is_running:
            tracker.track()
            

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

