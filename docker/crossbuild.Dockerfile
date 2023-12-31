FROM ubuntu:20.04

LABEL org.opencontainers.image.source https://github.com/berlingoqc/quintal

ENV DEBIAN_FRONTEND=noninteractive

COPY ./source.list /etc/apt/sources.list

WORKDIR /usr/src/app

RUN dpkg --add-architecture armhf && dpkg --add-architecture arm64 && \
	apt update -yq && apt upgrade -yq && apt install -yq crossbuild-essential-armhf crossbuild-essential-arm64 gcc-arm-linux-gnueabihf gcc libc6-armhf-cross libc6-dev-armhf-cross

RUN apt install -yq  gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu libc6-arm64-cross libc6-dev-arm64-cross

RUN DEBIAN_FRONTEND="noninteractive"  TZ="America/New_York" apt install -yq curl rsync openssh-client libdbus-1-dev pkg-config cmake
RUN apt install -yq protobuf-compiler
RUN apt install -yq libsqlite3-dev libsqlite3-dev:armhf libsqlite3-dev:arm64
RUN apt install -yq libssl-dev libssl-dev:armhf libssl-dev:arm64
RUN apt install -yq libdbus-1-dev libdbus-1-dev:armhf libdbus-1-dev:arm64


RUN apt install -yq gcc-arm* g++ g++-arm* g++-aarch*
RUN apt install -yq libboost-dev libboost-dev:arm64 libasio-dev libboost-system-dev libboost-system-dev:arm64

RUN apt install -yq libgstreamer1.0-dev libgstreamer1.0-dev:arm64 libgstreamer1.0-dev:armhf
RUN apt-get install -yq nlohmann-json3-dev

#RUN apt-get install -yq git && git clone https://github.com/paullouisageneau/libdatachannel.git && \
#	cd libdatachannel && git submodule update --init --recursive --depth 1 && \
#	mkdir build && cd build && CXX=arm-linux-gnueabi-g++ CC=arm-linux-gnueabi-gcc cmake .. && make && make install

RUN apt-get install -yq libopencv-dev

RUN apt-get install -yq git curl zip unzip tar

RUN git clone https://github.com/Microsoft/vcpkg.git && ./vcpkg/bootstrap-vcpkg.sh

RUN ./vcpkg/vcpkg install protobuf protobuf:arm-linux protobuf:arm64-linux
RUN ./vcpkg/vcpkg install libdatachannel libdatachannel:arm-linux libdatachannel:arm64-linux
RUN ./vcpkg/vcpkg install libdatachannel libdatachannel:arm-linux libdatachannel:arm64-linux
RUN ./vcpkg/vcpkg install boost-filesystem:arm64-linux boost-system:arm64-linux boost-asio[ssl]:arm64-linux boost-beast:arm64-linux
RUN ./vcpkg/vcpkg install boost-filesystem:arm-linux boost-system:arm-linux boost-asio[ssl]:arm-linux boost-beast:arm-linux
RUN ./vcpkg/vcpkg install boost-filesystem boost-system boost-asio[ssl] boost-beast
RUN ./vcpkg/vcpkg install nlohmann-json nlohmann-json:arm64-linux nlohmann-json:arm-linux
RUN apt-get -yq install flex bison
RUN apt-get -yq install nasm
RUN ./vcpkg/vcpkg install gstreamer gstreamer:arm64-linux gstreamer:arm-linux
