# CMakeList.txt : CMake project for mvlar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
    "${PROJECT_SOURCE_DIR}/libcontrol/include"
    "${PROJECT_SOURCE_DIR}/libfirmata/include"
)

set(PROTO_FILES ../proto/control.proto ../proto/msg_header.proto)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# Your source files
set(SOURCE_FILES ${PROTO_SRCS})


add_executable (quintal_control
    main.cpp

    camera_streamer.cpp
    camera_analysis.cpp

    webrtc_server.cpp

    ${SOURCE_FILES}
)

if(APPLE)
	set(LINK_THREAD Boost::thread)
elseif(UNIX)
	set(LINK_THREAD pthread rt boost_thread)
endif ()


target_include_directories(quintal_control PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(quintal_control PRIVATE
    LibDataChannel::LibDataChannel
    ${LINK_THREAD}
    libcontrol
    libfirmata
    ${PROTOBUF_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
    ${OpenCV_LIBS}
)