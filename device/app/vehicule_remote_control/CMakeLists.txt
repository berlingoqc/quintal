# CMakeList.txt : CMake project for mvlar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories ()

add_executable (quintal_control
    main.cpp
)

target_include_directories(quintal_control PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(quintal_control PRIVATE
    ${PROTOBUF_LIBRARY}
    ${LINK_THREAD}
    firmata
    vehicule
    utils
    analysis
    quintal_proto
    webrtc_server
)