FROM debian:bullseye-slim

LABEL org.opencontainers.image.source https://github.com/berlingoqc/quintal/pibuild

ENV DEBIAN_FRONTEND=noninteractive
ENV TZ="America/New_York"
ENV MBED_TLS_VERSION=3.0.0

WORKDIR /usr/src/app

RUN  apt update -yq && apt upgrade -yq

RUN apt install -yq build-essential \
	curl rsync openssh-client libdbus-1-dev pkg-config \
    protobuf-compiler libprotobuf-dev \
	libssl-dev \
	libdbus-1-dev \
	libasio-dev \
	libgstreamer1.0-dev \
	nlohmann-json3-dev \
	libopencv-dev \
	git curl zip unzip tar \
	ninja-build cmake

#RUN curl -L https://github.com/Kitware/CMake/releases/download/v3.27.2/cmake-3.27.2.tar.gz --output cmake-3.27.2.tar.gz && tar zxvf cmake-3.27.2.tar.gz && cd cmake-3.27.2 && ./bootstrap && make && make install
#ENV VCPKG_FORCE_SYSTEM_BINARIES 1
#RUN git clone https://github.com/Microsoft/vcpkg.git && ./vcpkg/bootstrap-vcpkg.sh
#RUN ./vcpkg/vcpkg install protobuf

#ENV MBED_TLS_VERSION=3.4.1
#RUN curl --location https://github.com/Mbed-TLS/mbedtls/archive/refs/tags/v${MBED_TLS_VERSION}.zip --output output.zip && \
#	unzip output.zip && cd mbedtls-${MBED_TLS_VERSION} && make && make install

RUN git clone https://github.com/paullouisageneau/libdatachannel.git && \
	cd libdatachannel && git submodule update --init --recursive --depth 1 && \
	mkdir -p build && cd build && cmake .. -DUSE_MBEDTLS=0 -DUSE_GNUTLS=0 -DUSE_NICE=0 -DCMAKE_BUILD_TYPE=Release && make && make install
