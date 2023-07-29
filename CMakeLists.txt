# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("quintal" CXX)
# Déclare les dépendences de buid
set(CMAKE_CXX_STANDARD 17)

#set( ASIO_INCLUDE_DIR "" CACHE PATH "The path containing asio/asio.hpp")
#add_definitions(-DASIO_STANDALONE)

enable_testing()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("debug mode")
endif()


include_directories(
    libutils
)
#add_definitions(-DWITH_STB_IMAGE)

link_directories(
)


#add_definitions(-DDEBUG_VIDEO_CLIENT)
#add_definitions(-DDEBUG_VIDEO_STAGING)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#find_package( OpenGL REQUIRED)
#find_package( OpenCV REQUIRED)
#find_package( Qt5Network REQUIRED)
#find_package( Qt5Gamepad REQUIRED)

include_directories( 
#	${ASIO_INCLUDE_DIR}
	./libutils
)

link_directories()

#set ( CMAKE_CXX_FLAGS -march=native)

# Include sub-projects.
add_subdirectory("libcontrol")
add_subdirectory("app")

