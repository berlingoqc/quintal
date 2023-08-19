#! /bin/bash

RELEASE_NAME=${1-:aarch64}

mkdir -p device/$RELEASE_NAME

cd device/build_$RELEASE_NAME && \
cmake .. -DCMAKE_BUILD_TYPE=Release && \
make quintal_control

mkdir -p bundle
# boost , opencv and libdatachannel shared lib
cp ./app/vehicule_remote_control/quintal_control ./bundle/
cp /usr/local/lib/libdatachannel.so.0.18.5 ./bundle/

(cd bundle && zip -r ../$RELEASE_NAME.zip .)

# executable
