#! /bin/bash

mkdir -p device/build_aarch64

cd device/build_aarch64 && \
cmake .. -DCMAKE_TOOLCHAIN_FILE=../../../vcpkg/scripts/buildsystems/vcpkg.cmake && \
make quintal_proto quintal_control

mkdir -p bundle
# boost , opencv and libdatachannel shared lib
cp ./app/vehicule_remote_control/quintal_control ./bundle/
cp /lib/aarch64-linux-gnu/libopencv_objdetect.so.4.2 ./bundle/
cp /lib/aarch64-linux-gnu/libopencv_calib3d.so.4.2 ./bundle/
cp /lib/aarch64-linux-gnu/libopencv_imgproc.so.4.2 ./bundle/
cp /lib/aarch64-linux-gnu/libboost_thread.so.1.71.0 ./bundle/
cp /lib/aarch64-linux-gnu/libboost_filesystem.so.1.71.0 ./bundle/
cp /usr/local/lib/libdatachannel.so.0.18.5 ./bundle/
cp /lib/aarch64-linux-gnu/libopencv_core.so.4.2 ./bundle/

zip -r bundle_aarch64.zip ./bundle/*

# executable
