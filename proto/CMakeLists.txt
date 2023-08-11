cmake_minimum_required(VERSION 3.10)
project(proto)

set(CMAKE_CXX_STANDARD 17)

# Define your .proto files
set(PROTO_FILES control.proto msg_header.proto)

# Generate the .h and .cpp files from the .proto files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# Your source files
set(SOURCE_FILES ${PROTO_SRCS})

# Create your library
add_library(${PROJECT_NAME} ${SOURCE_FILES})

# Include the directory where the protobuf generated files are 
# along with other include directories you might have
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Link the library with protobuf libraries
target_link_libraries(${PROJECT_NAME} ${PROTOBUF_LIBRARIES})