import numpy as np
from typing import List, Tuple
import supervision as sv

class Rect:
    x: int
    y: int
    width: int
    height: int

    def __init__(self, x: int, y: int, width: int, height: int) -> Rect: ...
    
    def into_xyxy(self) -> List[int]: ...


class BoundingBox:
    group_id: int
    rect: Rect
    confidence: float


class Entity:
    id: int
    bounding_box: BoundingBox
    predicted_location: Rect
    times_not_found: int


class GeneralPurposeTracker:
    def __init__(
        self, 
        model_weights: str, 
        video: str, 
        config_path: str = "config.json", 
        skip_frame: int = 2
    ) -> None:
        ...

    def track(self) -> None:
        ...

    @staticmethod
    def process_frame(frame: np.ndarray) -> np.ndarray:
        ...

    def predict(self) -> Tuple[any, np.ndarray]:  # Replace 'any' with the actual type returned by the model
        ...

    @staticmethod
    def prediction_to_points(result: any) -> Tuple[np.ndarray, np.ndarray, np.ndarray]:  # Replace 'any' with the actual type
        ...


class visualizer:
    def __init__(self, pallate: sv.ColorPalette) -> None:
        ...

    def annotate_detections(self, tracker_result: List[any]) -> None:  # Replace 'any' with the actual Entity type
        ...

    def show_frame(self, wait_time: int) -> None:
        ...

    @staticmethod
    def type_color(type: int) -> Tuple[int, int, int]:
        ...