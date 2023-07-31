cmake_minimum_required (VERSION 3.8)

include_directories (
    "${PROJECT_SOURCE_DIR}/libfirmata/include"
)

add_executable (example_firmata main.cpp)

if(APPLE)
elseif(UNIX)
	set(LINK_THREAD pthread rt boost_thread)
endif ()

target_link_libraries(example_firmata PRIVATE ${LINK_THREAD} libfirmata)