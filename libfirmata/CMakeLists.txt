# CMakeList.txt : Pour le librairie libardrone

cmake_minimum_required (VERSION 3.8)

project (libfirmata VERSION 0.0.1 LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set ( SRC src/serialport.cpp )

# Crée ma target de la librairie
add_library (libfirmata STATIC ${SRC})
target_link_libraries (libfirmata stdc++)