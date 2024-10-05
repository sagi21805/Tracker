#!/bin/sh
wd=$(pwd)
echo "Updating system..."
sudo apt update -y && sudo apt upgrade -y

echo "Adding necessery repositeries to apt..."
sudo add-apt-repository -y ppa:ubuntu-toolchain-r/test 
sudo add-apt-repository ppa:deadsnakes/ppa -y

echo "Installing necessery packages..."
sudo apt install clang libclang-dev git python3.11 python3.11-full
pip3.11 install -r $wd/requirements.txt

echo "Installing Rust..."
curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

echo "Installing the library!"
install_path=$(python3 -m site --user-site)
pip3 install maturin
python3 -m maturin build --release
pip3 install target/wheels/* --force-reinstall