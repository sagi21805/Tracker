import cv2
import numpy as np

class visualizer():

    def __init__(self) -> None:
        self.frame = np.zeros(0)

    def add_rectangle(self, p1, p2, type):
        cv2.rectangle(self.frame, p1, p2, visualizer.type_color(type), 3)
    
    def add_text(self, text, org):
        cv2.putText(self.frame, text, org, cv2.FONT_HERSHEY_COMPLEX, 2.0, (0, 255, 255), 2)

    def show(self, delay_secs=1):
        cv2.imshow("Visualization", self.frame)
        cv2.waitKey(delay_secs)

    def type_color(type: int):
        match type:
            case 0:
                return (255, 0, 0)
            case 1: 
                return (0, 0, 255)

