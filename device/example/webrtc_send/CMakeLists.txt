cmake_minimum_required (VERSION 3.8)

add_executable (example_webrtc main.cpp)

target_link_libraries(example_webrtc LibDataChannel::LibDataChannel)