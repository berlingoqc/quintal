SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_PROCESSOR arm)

SET(TOOLCHAIN_PREFIX arm-linux-gnueabihf)

# specify the cross compiler
SET(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
SET(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)

# where is the target environment
SET(CMAKE_FIND_ROOT_PATH /usr/${TOOLCHAIN_PREFIX} /usr/lib/${TOOLCHAIN_PREFIX} /usr/include/${TOOLCHAIN_PREFIX})


SET(OPENSSL_ROOT_DIR /usr/lib/arm-linux-gnueabihf)
SET(OPENSSL_INCLUDE_DIR /usr/include/arm-linux-gnueabihf/openssl)
SET(PCAP_LIBRARY /usr/lib/arm-linux-gnueabihf/libpcap.so)

# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)