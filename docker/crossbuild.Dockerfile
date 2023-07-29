FROM ubuntu:20.04

COPY ./source.list /etc/apt/sources.list

WORKDIR /usr/src/app

RUN dpkg --add-architecture armhf &&  dpkg --add-architecture arm64 && \
	apt update -yq && apt upgrade -yq && apt install -yq gcc-arm-linux-gnueabihf gcc libc6-armhf-cross libc6-dev-armhf-cross

RUN apt install -yq  gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu libc6-arm64-cross libc6-dev-arm64-cross

RUN DEBIAN_FRONTEND="noninteractive"  TZ="America/New_York" apt install -yq curl rsync openssh-client libdbus-1-dev pkg-config cmake
RUN apt install -yq protobuf-compiler
RUN apt install -yq libsqlite3-dev libsqlite3-dev:armhf libsqlite3-dev:arm64
RUN apt install -yq libssl-dev libssl-dev:armhf libssl-dev:arm64
RUN apt install -yq libdbus-1-dev libdbus-1-dev:armhf libdbus-1-dev:arm64


RUN apt install -yq gcc-arm* g++ g++-arm* g++-aarch*