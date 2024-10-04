#!/bin/sh
wd=$(pwd)

cd ~    
sudo apt update -y && sudo apt upgrade -y
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test 
sudo apt install -y gcc-13 g++-13 
sudo apt install -y cmake make git libgtk2.0-dev
sudo apt install -y pkg-config ffmpeg libavformat-dev libavcodec-dev libswscale-dev
sudo apt install clang libclang-dev
sudo add-apt-repository ppa:deadsnakes/ppa -y
sudo apt install python3.11 -y
sudo apt install python3.11-full -y
pip3.11 install -r $wd/requirements.txt
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

install_path=$(python3 -m site --user-site)
pip3 install maturin
python3 -m maturin build --release
pip3 install target/wheels/* --force-reinstall