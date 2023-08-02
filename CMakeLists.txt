cmake_minimum_required (VERSION 3.8)

project ("quintal" CXX)

set(CMAKE_CXX_STANDARD 20)

SET ( WEBSOCKETPP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/websocketpp" CACHE PATH "Directory for websocketpp" )

enable_testing()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("debug mode")
endif()

find_package(Boost REQUIRED COMPONENTS system)
find_package(libdatachannel REQUIRED)
find_package(OpenCV REQUIRED)

#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

find_library(GSTREAMER_LIBRARY
    NAMES GStreamer
    PATHS /Library/Frameworks/GStreamer.framework
    REQUIRED
)

# Find GStreamer headers
find_path(GSTREAMER_INCLUDE_DIR
    NAMES gst/gst.h
    PATHS /Library/Frameworks/GStreamer.framework/Headers
    REQUIRED
)

link_directories(/Library/Frameworks/GStreamer.framework/Libraries)

SET(CMAKE_C_LINK_EXECUTABLE ${CMAKE_CXX_LINK_EXECUTABLE})

include_directories(
	${Boost_INCLUDE_DIRS}
    libutils
	libfirmata
	${GSTREAMER_INCLUDE_DIR}
    ${OpenCV_INCLUDE_DIRS}
)

add_subdirectory("libfirmata")
add_subdirectory("libcontrol")
add_subdirectory("app")

add_subdirectory("example/firmata")

