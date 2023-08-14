
include_directories(
    ${PROJECT_SOURCE_DIR}/lib/firmata/includes
	${CMAKE_CURRENT_SOURCE_DIR}/includes
	
	${PROJECT_SOURCE_DIR}/build/lib/quintal_proto
    "${PROTOBUF_INCLUDE_DIR}"
    "${CMAKE_CURRENT_BINARY_DIR}"

)

set ( SRC 
	src/motor.cpp
	src/dc_car.cpp
	src/car_builder.cpp
	src/firmata_iot_control.cpp
)

# Crée ma target de la librairie
add_library (vehicule STATIC ${SRC})
