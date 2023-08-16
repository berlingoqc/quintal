
cmake_minimum_required (VERSION 3.8)

project("webrtc_server" VERSION 0.0.1 LANGUAGES CXX)


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)

set ( SRC src/camera_streamer.cpp src/webrtc_server.cpp src/udp_sink_server.cpp)

add_library(${PROJECT_NAME} STATIC ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC
	${LINK_THREAD}
	
    ${LIBDATACHANNEL_LIBRARIES}
    ${GSTREAMER_LIBRARIES}
    ${OpenCV_LIBS}
)

target_include_directories(${PROJECT_NAME} PUBLIC includes/)