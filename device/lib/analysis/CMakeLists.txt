
project("analysis" VERSION 0.0.1 LANGUAGES CXX)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/includes
)

set ( SRC 
	src/video_analysis.cpp
)

# Crée ma target de la librairie
add_library (${PROJECT_NAME} STATIC ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC utils ${OpenCV_LIBS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)