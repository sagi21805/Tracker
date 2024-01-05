from python_utils.Tracker import Tracker
from pynput import keyboard
import threading
#TEST CODE DONT TAKE SERIOUSLY


tracker = Tracker("bumperWeights.pt", "/home/sagi21805/dcmp4.mp4")
is_running = True

def main_code():
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
listener = keyboard.Listener(on_press=on_press)

listener_thread = threading.Thread(target=listener.start)

if __name__ == "__main__":
    listener_thread.start()
    main_code()
