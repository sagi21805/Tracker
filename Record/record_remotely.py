import os
import subprocess
from pynput import keyboard
import time
from connection_utils import RemoteHost
from pathlib import Path

remote_pid = None
username = "ubuntu"
script_path = Path(f"/home/{username}/camera/record.py")  # File path on the remote machine
local_destination = Path(f"Record")  # 
record_host = RemoteHost("ubuntu@10.100.102.24")

def start_remote_recording():
    global remote_pid
    print("Starting remote recording...")
    remote_pid = record_host.run_python_script(script_path)
    print(f"Remote recording started with PID: {remote_pid}")

def stop_and_transfer():
    global remote_pid
    if remote_pid:
        print("Stopping recording on the remote machine...")
        # Stop the recording by sending SIGINT (Ctrl+C equivalent)
        record_host.kill_process(remote_pid)
        print("Recording stopped, transferring the file...")
        # Transfer the file from the remote machine to the local machine
        record_host.transfer_files(Path("/home/ubuntu/*.avi"), local_destination)
        record_host.remove_file(Path("/home/ubuntu/*.avi"))
        print("File transfer completed.")
    else:
        print("No remote recording process found!")


def on_press(key):
    try:
        if key.char == 'q':
            print("Key 'q' pressed, stopping the recording...")
            stop_and_transfer()
            return False  # Stop listener after key press
    except AttributeError:
        pass

def main():
    # Get the PID of the recording process on the remote machine
    start_remote_recording()

    print("Press 'q' to stop the recording and transfer the file.")
    
    # Wait for 'q' key press to stop recording and transfer the file

    # Use pynput to listen for keypresses
    with keyboard.Listener(on_press=on_press) as listener:
        listener.join()  # Wait until the listener is stopped    

if __name__ == "__main__":
    main()

