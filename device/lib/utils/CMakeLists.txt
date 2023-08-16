
cmake_minimum_required (VERSION 3.8)

project("utils" VERSION 0.0.1 LANGUAGES CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/includes)

set ( SRC src/file_manager.cpp )

add_library(${PROJECT_NAME} STATIC ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC
	${PROTOBUF_LIBRARY}
	Boost::filesystem
	quintal_proto
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)