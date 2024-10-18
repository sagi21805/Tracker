from numpy import ndarray
from supervision.draw.color import ColorPalette, Color
from tracker import GeneralPurposeTracker


class RobotTracker(GeneralPurposeTracker):

    def process_frame(self, frame: ndarray) -> ndarray:
        # Custom processing logic for the frame
        return frame[0:460]  # Assuming you want to return the first 460 rows

robot_tracker = RobotTracker(
    "assets\\bumperWeights.pt", 
    "assets\\TestVideo.mp4",
    ColorPalette([Color.BLUE, Color.RED, Color.BLACK]),
    skip_frame=1,
    entities_per_class=[3, 3]
)

if __name__ == "__main__": 
    while True:
        robot_tracker.track() # Start the main Tracking code
