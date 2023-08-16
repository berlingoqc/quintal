
project("vehicule" VERSION 0.0.1 LANGUAGES CXX)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}/includes
)

set ( SRC 
	src/motor.cpp
	src/dc_car.cpp
	src/car_builder.cpp
	src/firmata_iot_control.cpp
)

# Crée ma target de la librairie
add_library (vehicule STATIC ${SRC})

target_link_libraries(${PROJECT_NAME} PUBLIC firmata utils)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/includes)