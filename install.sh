#!/bin/sh
wd=$(pwd)

cd ~    
sudo apt update -y && sudo apt upgrade -y
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test 
sudo apt install -y gcc-13 g++-13
sudo apt install -y cmake make git libgtk2.0-dev
sudo apt install -y pkg-config ffmpeg libavformat-dev libavcodec-dev libswscale-dev
apt install libopencv-dev clang libclang-dev

sudo add-apt-repository ppa:deadsnakes/ppa -y
sudo apt install python3.11 -y
sudo apt install python3.11-full -y
sudo pip install -r $wd/requirements.txt

# echo Installing Rust \& Cargo
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
cargo build