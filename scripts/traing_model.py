from ultralytics import YOLO
import torch
def main():
    # Load a pretrained YOLO model (recommended for training)
    
    model = YOLO("yolo11s.pt")
    torch.cuda.empty_cache()
    results = model.train(data=r"/home/sagi/Tracker/Data/Roobots Dataset for FRC Rooobots.v7i.yolov11/data.yaml", epochs=100, batch=5)

    # Evaluate the model's performance on the validation set
    results = model.val()

    model.save("BumperModel.pt")

if __name__ == '__main__':
    main()
