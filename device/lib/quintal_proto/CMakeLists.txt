

include_directories (
    "${PROTOBUF_INCLUDE_DIR}"
)

SET (PROTO_FILES ../../../proto/control.proto ../../../proto/msg_header.proto ../../../proto/config.proto)

PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

set(SOURCE_FILES ${PROTO_SRCS})


add_library(quintal_proto ${SOURCE_FILES})

# Include the directory where the protobuf generated files are 
# along with other include directories you might have
target_include_directories(quintal_proto PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# Link the library with protobuf libraries
target_link_libraries(quintal_proto ${PROTOBUF_LIBRARIES})
