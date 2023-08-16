# CMakeList.txt : Pour le librairie libardrone

cmake_minimum_required (VERSION 3.8)

project (firmata VERSION 0.0.1 LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)

set ( SRC src/firmata_client.cpp )

# Crée ma target de la librairie
add_library (${PROJECT_NAME} STATIC ${SRC})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)