#! /bin/bash

mkdir build-arm64
mkdir build-armvf

cd build-arm64 && cmake .. && make
cd build-armvf && cmake .. && make