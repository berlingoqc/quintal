FROM ubuntu:20.04

LABEL org.opencontainers.image.source https://github.com/berlingoqc/quintal/pibuild

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /usr/src/app

RUN  apt update -yq && apt upgrade -yq

RUN apt install -yq  gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu libc6-arm64-cross libc6-dev-arm64-cross

RUN DEBIAN_FRONTEND="noninteractive"  TZ="America/New_York" apt install -yq curl rsync openssh-client libdbus-1-dev pkg-config cmake
RUN apt install -yq protobuf-compiler
RUN apt install -yq libssl-dev
RUN apt install -yq libdbus-1-dev

RUN apt install -yq gcc g++
RUN apt install -yq libboost-dev libasio-dev libboost-system-dev 

RUN apt install -yq libgstreamer1.0-dev 
RUN apt-get install -yq nlohmann-json3-dev

RUN apt-get install -yq git && git clone https://github.com/paullouisageneau/libdatachannel.git && \
	cd libdatachannel && git submodule update --init --recursive --depth 1 && \
	mkdir build && cd build && cmake .. && make && make install

RUN apt-get install -yq libopencv-dev

RUN apt-get install -yq git curl zip unzip tar

RUN git clone https://github.com/Microsoft/vcpkg.git && ./vcpkg/bootstrap-vcpkg.sh

RUN ./vcpkg/vcpkg install protobuf
#RUN ./vcpkg/vcpkg install libdatachannel libdatachannel:arm-linux libdatachannel:arm64-linux
#RUN ./vcpkg/vcpkg install libdatachannel libdatachannel:arm-linux libdatachannel:arm64-linux
#RUN ./vcpkg/vcpkg install boost-filesystem:arm64-linux boost-system:arm64-linux boost-asio[ssl]:arm64-linux boost-beast:arm64-linux
#RUN ./vcpkg/vcpkg install boost-filesystem:arm-linux boost-system:arm-linux boost-asio[ssl]:arm-linux boost-beast:arm-linux
#RUN ./vcpkg/vcpkg install boost-filesystem boost-system boost-asio[ssl] boost-beast
#RUN ./vcpkg/vcpkg install nlohmann-json nlohmann-json:arm64-linux nlohmann-json:arm-linux
#RUN apt-get -yq install flex bison
#RUN apt-get -yq install nasm
#RUN ./vcpkg/vcpkg install gstreamer gstreamer:arm64-linux gstreamer:arm-linux
