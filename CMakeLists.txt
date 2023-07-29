cmake_minimum_required (VERSION 3.8)

project ("quintal" CXX)

set(CMAKE_CXX_STANDARD 20)

SET ( WEBSOCKETPP_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/websocketpp" CACHE PATH "Directory for websocketpp" )

enable_testing()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	message("debug mode")
endif()

find_package(Boost REQUIRED COMPONENTS system)

include_directories(
	${Boost_INCLUDE_DIRS}
    libutils
)

add_subdirectory("libcontrol")
add_subdirectory("app")

if(APPLE)
elseif(UNIX)
	add_subdirectory("firmatac")
endif()

