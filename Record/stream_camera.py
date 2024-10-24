import socket
import cv2
import pickle
import struct
import threading

# Shared variables for frame storage
frames = {"Camera 1": None, "Camera 2": None}
lock = threading.Lock()

# Function to receive and display stream from a single camera
def receive_stream(ip, port, camera_name):
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((ip, port))  # Connect to the Raspberry Pi's IP and the respective port

    data = b""
    payload_size = struct.calcsize("L")

    while True:
        # Retrieve message size
        while len(data) < payload_size:
            packet = client_socket.recv(4096)
            if not packet:
                break
            data += packet

        packed_msg_size = data[:payload_size]
        data = data[payload_size:]
        msg_size = struct.unpack("L", packed_msg_size)[0]

        # Retrieve the actual frame data
        while len(data) < msg_size:
            data += client_socket.recv(4096)

        frame_data = data[:msg_size]
        data = data[msg_size:]

        # Lock to safely update the shared frames dictionary
        with lock:
            frames[camera_name] = pickle.loads(frame_data)

# Function to display frames in a separate thread
def display_frames():
    while True:
        with lock:
            if frames["Camera 1"] is not None:
                cv2.imshow("Camera 1", frames["Camera 1"])
            if frames["Camera 2"] is not None:
                cv2.imshow("Camera 2", frames["Camera 2"])

        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cv2.destroyAllWindows()

# Raspberry Pi's IP address
raspberry_pi_ip = '10.100.102.24'  # Replace with your Raspberry Pi's actual IP address

# Start receiving streams from both cameras on different ports
threading.Thread(target=receive_stream, args=(raspberry_pi_ip, 8089, "Camera 1")).start()
threading.Thread(target=receive_stream, args=(raspberry_pi_ip, 8090, "Camera 2")).start()

# Start displaying frames in the main thread
display_frames()
