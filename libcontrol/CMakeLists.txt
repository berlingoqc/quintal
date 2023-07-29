# CMakeList.txt : Pour le librairie libardrone
cmake_minimum_required (VERSION 3.8)

project (libcontrol VERSION 0.0.1 LANGUAGES CXX)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

set ( SRC src/quintal.cpp )

# Crée ma target de la librairie
add_library (libcontrol STATIC ${SRC})

target_link_libraries (libcontrol stdc++)

