#!/bin/sh
cmake -B ./build

cd build 

make -j4

mv libTracker.so ../libTracker.so

echo "[INFO]: Finished building running python!"

cd ..

sudo python3.11 main.py 