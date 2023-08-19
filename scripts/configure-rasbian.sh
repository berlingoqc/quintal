#! /bin/bash

# /etc/apt/source.list
# deb  http://mirror.umd.edu/raspbian/raspbian buster main contrib non-free rpi

sudo apt-get update
sudo apt-get upgrade


sudo apt-get install build-essential ninja-build cmake protobuf-compiler libprotobuf-dev libssl-dev libopencv-dev curl zip unzip tar libboost-dev libasio-dev libboost-system-dev libboost-filesystem-dev libboost-thread-dev libgstreamer1.0-dev nlohmann-json3-dev