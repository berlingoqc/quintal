cmake_minimum_required (VERSION 3.8)

include_directories (
    "${PROJECT_SOURCE_DIR}/lib/firmata/includes"
)

add_executable (example_firmata main.cpp)


target_link_libraries(example_firmata PRIVATE ${LINK_THREAD} firmata)