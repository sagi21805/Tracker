#! /bin/bash

install_path=$(python3 -m site --user-site)
python3 -m maturin build --release
pip3 install target/wheels/* --force-reinstall