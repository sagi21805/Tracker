import cv2
import numpy as np

class visualizer():

    def __init__(self) -> None:
        self.frame = np.zeros()

    def add_rectangle(self, p1, p2, color):
        cv2.rectangle(self.frame, p1, p2, color)
    
    def add_text(self, text, org, color):
        cv2.putText(self.frame, text, org, cv2.FONT_HERSHEY_COMPLEX, 1.0, color)

    def show(self, delay_secs=1):
        cv2.imshow("Visualization")
        cv2.waitKey(delay_secs)