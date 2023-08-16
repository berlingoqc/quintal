FROM ubuntu:20.04

LABEL org.opencontainers.image.source https://github.com/berlingoqc/quintal/pibuild

ENV DEBIAN_FRONTEND=noninteractive

WORKDIR /usr/src/app

RUN  apt update -yq && apt upgrade -yq

RUN apt install -yq  gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu libc6-arm64-cross libc6-dev-arm64-cross

RUN DEBIAN_FRONTEND="noninteractive"  TZ="America/New_York" apt install -yq curl rsync openssh-client libdbus-1-dev pkg-config
RUN apt install -yq protobuf-compiler
RUN apt install -yq libssl-dev
RUN apt install -yq libdbus-1-dev

RUN apt install -yq gcc g++
RUN apt install -yq libboost-dev libasio-dev libboost-system-dev libboost-filesystem-dev libboost-thread-dev
RUN apt install -yq libgstreamer1.0-dev 
RUN apt-get install -yq nlohmann-json3-dev

RUN curl -L https://github.com/Kitware/CMake/releases/download/v3.27.2/cmake-3.27.2.tar.gz --output cmake-3.27.2.tar.gz && tar zxvf cmake-3.27.2.tar.gz && cd cmake-3.27.2 && ./bootstrap && make && make install

RUN apt-get install -yq git && git clone https://github.com/paullouisageneau/libdatachannel.git && \
	cd libdatachannel && git submodule update --init --recursive --depth 1 && \
	mkdir build && cd build && cmake .. && make && make install


RUN apt-get install -yq libopencv-dev

RUN apt-get install -yq git curl zip unzip tar

RUN apt-get -yq install ninja-build

ENV VCPKG_FORCE_SYSTEM_BINARIES 1

RUN git clone https://github.com/Microsoft/vcpkg.git && ./vcpkg/bootstrap-vcpkg.sh

RUN ./vcpkg/vcpkg install protobuf

#RUN apt-get -yq install gcc-arm-linux-gnueabihf g++-arm-linux-gnueabihf
#RUN dpkg --add-architecture armhf
#RUN apt-get install libssl-dev:armhf libboost-dev:armhf libasio-dev:armhf libboost-system-dev:armhf libboost-filesystem-dev:armhf libboost-thread-dev:armhf libgstreamer1.0-dev:armhf 

