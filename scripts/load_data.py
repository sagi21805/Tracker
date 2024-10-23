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
        print(self.train_path, self.test_path, self.valid_path)
       
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
        new_class_id = self.mapping.get(components[CLASS_ID], components[CLASS_ID])
        components[CLASS_ID] = new_class_id
        return ' '.join(components)
 

    def change_class_mapping(self):
        all_labels = self.train_labels + self.test_labels + self.valid_labels
        for label_file_path in all_labels:
            with open(label_file_path, 'r') as label_file:
                content = label_file.read()
            new_content = '\n'.join([self.new_class_map(box) for box in content.split("\n")])
            with open(label_file_path, 'w') as label_file:
                content = label_file.write(new_content)
    
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
        


# red = 0 and 3 
# blue = 1 and 2
mapping_first = {
    '0': '0',
    '1': '1',
    '2': '1',
    '3': '0'
}

first = DataSet(r"C:\Users\sagi\Downloads\robot-bumpers.v7i.yolov11", mapping=mapping_first)

mapping_second = {
    '1' : '0',
    '0' : '1'
}

second = DataSet(r"C:\Users\sagi\Downloads\robot-bumpers.v1i.yolov11", mapping=mapping_second)


thired = DataSet(r"C:\Users\sagi\Downloads\robot-detect.v3i.yolov11", mapping=mapping_second)

forth = DataSet(r"C:\Users\sagi\Downloads\bumpers-detection-v2.v5i.yolov11", mapping=mapping_second)

first.combine(second)
first.combine(thired)
first.combine(forth)