from typing import Optional
from pathlib import Path
import yaml
import shutil

CLASS_ID = 0

class DataSet:

    def __init__(self, path: str, mapping: Optional[dict[str, str]] = None) -> None:
        self.data_path = Path(path)
        self.yaml = self.data_path / 'data.yaml'
        self.train_path = self.data_path / 'train'
        self.test_path = self.data_path / 'test' if (self.data_path / 'test').exists() else None
        self.valid_path = self.data_path / 'valid' if (self.data_path / 'valid').exists() else None
        print(self.train_path, "##", self.test_path, "##", self.valid_path)
       
        self.train_labels = [f.resolve() for f in (self.train_path / 'labels').iterdir() if f.is_file()]
        self.test_labels = [f.resolve() for f in (self.test_path / 'labels').iterdir() if f.is_file()] if self.test_path else []
        self.valid_labels =  [f.resolve() for f in (self.valid_path / 'labels').iterdir() if f.is_file()] if self.valid_path else []
        self.train_imgs = [f.resolve() for f in (self.train_path / 'images').iterdir() if f.is_file()]
        self.test_imgs = [f.resolve() for f in (self.test_path / 'images').iterdir() if f.is_file()] if self.test_path else []
        self.valid_imgs =  [f.resolve() for f in (self.valid_path / 'images').iterdir() if f.is_file()] if self.valid_path else []
        with self.yaml.open('r') as file:
            self.config = yaml.safe_load(file)
        self.mapping = mapping if mapping != None else {item: item for item in self.config['names']} 
        print(self.mapping)
        self.change_class_mapping()


    def new_class_map(self, old_box: str):
        components = old_box.split(' ')
        if components[CLASS_ID] in self.mapping.keys():
            new_class_id = self.mapping.get(components[CLASS_ID], components[CLASS_ID])
            components[CLASS_ID] = new_class_id
            return ' '.join(components)
        else: 
            return ""

    def change_class_mapping(self):
        all_labels = self.train_labels + self.test_labels + self.valid_labels
        for label_file_path in all_labels:
            with open(label_file_path, 'r') as label_file:
                content = label_file.read()
            new_content = '\n'.join([s for s in [self.new_class_map(box) for box in content.split("\n")] if s])
            with open(label_file_path, 'w') as label_file:
                label_file.write(new_content)
    
    def combine(self, dataset: 'DataSet'):

        for train_label_path, train_img_path in zip(dataset.train_labels, dataset.train_imgs):
            new_label_name = f"combined_{train_label_path.stem}.txt"
            new_image_name = f"combined_{train_img_path.stem}.jpg"
            shutil.move(train_label_path, str(self.train_path / 'labels' / new_label_name))
            shutil.move(train_img_path, str(self.train_path / 'images' / new_image_name))

        for test_label_path, test_img_path in zip(dataset.test_labels, dataset.test_imgs):
            new_label_name = f"combined_{test_label_path.stem}.txt"
            new_image_name = f"combined_{test_img_path.stem}.jpg"
            shutil.move(test_label_path, str(self.test_path / 'labels' / new_label_name))
            shutil.move(test_img_path, str(self.test_path / 'images' / new_image_name))

        for valid_label_path, valid_img_path in zip(dataset.valid_labels, dataset.valid_imgs):
            new_label_name = f"combined_{valid_label_path.stem}.txt"
            new_image_name = f"combined_{valid_img_path.stem}.jpg"
            shutil.move(valid_label_path, str(self.valid_path / 'labels' / new_label_name))
            shutil.move(valid_img_path, str(self.valid_path / 'images' / new_image_name))
        


# Blue = 0 
# Red = 1
# 03 red 12 blue
mapping_first = {
    '0': '0',
    '1': '1',
}

mapping_second = {
    '1' : '0',
    '0' : '1'
}

mapping_thired = {
    "0" : "1",
    "1" : "0",
    "2" : "0",
    "3" : "1"
}

mapping_sixth = {
    "1" : "0",
    "2" : "1"
}



first = DataSet("Data/Roobots Dataset for FRC Rooobots.v7i.yolov11", mapping_first)
second = DataSet("Data/robot-detect.v3i.yolov11", mapping_first)
thired = DataSet("Data/robot-bumpers.v7i.yolov11", mapping_thired)
forth = DataSet("Data/robot-bumpers.v1i.yolov11", mapping_first)
fifth = DataSet("Data/Robot Detection.v11-yolov8-10-18.yolov11", mapping_first)
sixth = DataSet("Data/HyperClock.v9i.yolov11", mapping_sixth)
seventh = DataSet("Data/bumpers-detection-v2.v5i.yolov11", mapping_first)
# eighth = DataSet("Data/Bumper Color Classification.v1i.multiclass", )
first.combine(second)
first.combine(thired)
first.combine(forth)
first.combine(fifth)
first.combine(sixth)
first.combine(seventh)

