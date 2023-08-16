cmake_minimum_required (VERSION 3.8)

project ("quintal" CXX)

set(CMAKE_CXX_STANDARD 20)

SET ( WEBSOCKETPP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/websocketpp" CACHE PATH "Directory for websocketpp" )

SET ( LIBDATACHANNEL_LIBRARIES LibDataChannel::LibDataChannel)

enable_testing()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("debug mode")
endif()


find_package(LibDataChannel REQUIRED)
find_package(OpenCV REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Protobuf REQUIRED)
find_package(Protobuf CONFIG REQUIRED)

if (APPLE)
    find_package(Boost REQUIRED COMPONENTS system thread filesystem)
    find_library(GSTREAMER_LIBRARIES
        NAMES GStreamer
        PATHS /Library/Frameworks/GStreamer.framework
        REQUIRED
    )

    # Find GStreamer headers
    find_path(GSTREAMER_INCLUDE_DIRS
        NAMES gst/gst.h
        PATHS /Library/Frameworks/GStreamer.framework/Headers
        REQUIRED
    )

    link_directories(${GSTREAMER_LIBRARIES}/Libraries)

	set(LINK_THREAD Boost::thread)

else()
    find_package(Boost REQUIRED COMPONENTS system filesystem)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)

	set(LINK_THREAD pthread rt boost_thread)
endif()

SET(CMAKE_C_LINK_EXECUTABLE ${CMAKE_CXX_LINK_EXECUTABLE})

include_directories(
	${Boost_INCLUDE_DIRS}
	firmata
    quintal_proto
    webrtc_server
    vehicule
    utils
    analysis
    protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite
	${GSTREAMER_INCLUDE_DIRS}
    ${OpenCV_INCLUDE_DIRS}

	"${CMAKE_CURRENT_BINARY_DIR}/lib/quintal_proto"
    "${PROTOBUF_INCLUDE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

add_subdirectory("lib")
add_subdirectory("example")
add_subdirectory("app")

