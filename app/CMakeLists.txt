# CMakeList.txt : CMake project for mvlar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
    "${PROJECT_SOURCE_DIR}/libcontrol/include"
)

add_executable (quintal_control main.cpp)

#set( LINK_FS stdc++fs pthread)

# Ajoute le lien a ma librairie
target_link_libraries (quintal_control libcontrol) 