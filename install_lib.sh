#! /bin/bash

install_path=$(python3 -m site --user-site)
pip3 install maturin
python3 -m maturin build --release
pip3 install target/wheels/* --force-reinstall