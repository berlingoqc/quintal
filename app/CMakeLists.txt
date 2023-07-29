# CMakeList.txt : CMake project for mvlar, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

include_directories (
    "${PROJECT_SOURCE_DIR}/libcontrol/include"
)

add_executable (quintal_control main.cpp )

if(APPLE)
elseif(UNIX)
	set(LINK_THREAD pthread)

    include_directories (
        "${PROJECT_SOURCE_DIR}/firmatac/includes"
    )

    set ( LINK_FIRMATAC firmatac)

endif ()

target_include_directories(quintal_control PRIVATE ${WEBSOCKETPP_INCLUDE_DIR})
target_link_libraries(quintal_control PRIVATE ${Boost_LIBRARIES} ${LINK_THREAD} ${LINK_FIRMATAC} libcontrol)