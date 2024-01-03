#!/bin/sh
cmake -B ./build
cd build 
make -j4
mv libTracker.so ../libTracker.so
echo "[INFO]: Finished building"
# Use double quotes around variables to prevent word splitting and globbing
while true; do
    read -p "Do you want to run Python? (y/n): " yn
    case $yn in
        [Yy]* ) 
            cd ..
            python3 main.py  # Replace 'main.py' with your actual Python script filename
            break;;
        [Nn]* ) 
            echo "Exiting."
            exit;;
        * ) 
            echo "Please answer yes or no.";;
    esac
done


