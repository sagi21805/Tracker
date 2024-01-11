#!/bin/sh
wd=$(pwd)

cd ~    
sudo apt update -y && sudo apt upgrade -y
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test 
sudo apt install -y gcc-13 g++-13
sudo apt install -y cmake make git libgtk2.0-dev
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-13 60
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-13 60  
sudo apt install -y pkg-config ffmpeg libavformat-dev libavcodec-dev libswscale-dev
git clone https://github.com/opencv/opencv.git
cd opencv
cmake -B build 
cd build 
make -j5 
sudo make install
cd ~
git clone https://github.com/nlohmann/json.git
cd json
cmake -B build
cd build
make -j5
sudo make install

cd ~
sudo add-apt-repository ppa:deadsnakes/ppa -y
sudo apt install python3.11 -y
sudo apt install python3.11-full -y
python3.11 -m venv Tracker-Venv
source ~/Tracker-Venv/bin/activate
sudo pip install -r $wd/requirements.txt