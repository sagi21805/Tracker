#!/bin/sh
sudo apt update -y && sudo apt upgrade -y
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test 
sudo apt install -y gcc-11 g++-11 cmake make git libgtk2.0-dev
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-11 50
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-11 50  
sudo apt install -y pkg-config ffmpeg libavformat-dev libavcodec-dev libswscale-dev
cd ~    
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