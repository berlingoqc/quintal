cmake_minimum_required (VERSION 3.8)

add_executable (example_gstreamercv main.cpp)

target_link_libraries(example_gstreamercv ${GSTREAMER_LIBRARIES} ${OpenCV_LIBS})