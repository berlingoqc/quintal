# CMakeList.txt : CMake project for mvlar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
    "${PROJECT_SOURCE_DIR}/libcontrol/include"
    "${PROJECT_SOURCE_DIR}/libfirmata/include"
)

add_executable (quintal_control
    main.cpp
    camera_streamer.cpp
    camera_analysis.cpp
)

if(APPLE)
	set(LINK_THREAD Boost::thread)
elseif(UNIX)
	set(LINK_THREAD pthread rt boost_thread)
endif ()

target_link_libraries(quintal_control PRIVATE
    ${LINK_THREAD}
    libcontrol
    libfirmata
    ${GSTREAMER_LIBRARIES}
    ${OpenCV_LIBS}
    LibDataChannel::LibDataChannel
)