# CMakeList.txt : CMake project for mvlar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
    "${PROJECT_SOURCE_DIR}/libcontrol/include"
    "${PROJECT_SOURCE_DIR}/libfirmata/include"
    "${PROTOBUF_INCLUDE_DIR}"

)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ../proto/control.proto ../proto/msg_header.proto)

add_executable (quintal_control
    main.cpp

    camera_streamer.cpp
    camera_analysis.cpp

    webrtc_server.cpp

    ${PROTO_SRCS}
    ${PROTO_HDRS}
)

if(APPLE)
	set(LINK_THREAD Boost::thread)
elseif(UNIX)
	set(LINK_THREAD pthread rt boost_thread)
endif ()


target_include_directories(quintal_control PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(quintal_control PRIVATE
    ${PROTOBUF_LIBRARY}
    LibDataChannel::LibDataChannel
    ${LINK_THREAD}
    libcontrol
    libfirmata

    ${GSTREAMER_LIBRARIES}
    ${OpenCV_LIBS}

)