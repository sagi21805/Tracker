from ultralytics import YOLO

def main():
    # Load a pretrained YOLO model (recommended for training)
    model = YOLO("yolo11s.pt")

    # Train the model on the COCO8 example dataset for 100 epochs
    results = model.train(data=r"C:\Users\sagi\Downloads\robot-bumpers.v7i.yolov11\data.yaml", epochs=15)

    # Evaluate the model's performance on the validation set
    results = model.val()

    model.save("saved_model.pt")

if __name__ == '__main__':
    main()