# CMakeList.txt : CMake project for mvlar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
    "${PROJECT_SOURCE_DIR}/lib/firmata/includes"
    "${PROJECT_SOURCE_DIR}/lib/webrtc_server/includes"
    "${PROJECT_SOURCE_DIR}/lib/vehicule/includes"
    "${PROJECT_SOURCE_DIR}/lib/utils/includes"
    "${PROJECT_SOURCE_DIR}/lib/analysis/includes"
)

add_executable (quintal_control
    main.cpp
)

target_include_directories(quintal_control PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

target_link_libraries(quintal_control PRIVATE
    ${PROTOBUF_LIBRARY}
    firmata
    vehicule
    utils
    analysis
    quintal_proto
    webrtc_server
)