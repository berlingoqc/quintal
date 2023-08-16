#include <ostream>
#include "car_builder.hpp"

std::map<std::string, std::shared_ptr<IotControl>> CarBuilder::getControl(const google::protobuf::Map<std::string, IoTControlConfig>& config) {
	auto map = std::map<std::string, std::shared_ptr<IotControl>>();

	for (const auto &pair : config) {
		auto config = pair.second;

		map[pair.first] = std::make_shared<FirmataIotControl>(config.firmata().device_name());
	}	

	return map;
}

CarBuilder::CarBuilder(std::map<std::string, std::shared_ptr<IotControl>>& controls) : controls_(controls) {}

std::shared_ptr<Car> CarBuilder::buildCar(const CarConfig& config) {

	auto steering_config = config.steering();
	auto throtle_config = config.throtle();

	auto steering_control = this->controls_.at(steering_config.iot_control());
	auto throtle_control = this->controls_.at(throtle_config.iot_control());

	DCMotor steering_motor(steering_control, steering_config.dc().int1(), steering_config.dc().int2(), steering_config.dc().power());
	DCMotor throtle_motor(throtle_control, throtle_config.dc().int1(), throtle_config.dc().int2(), throtle_config.dc().power());

	auto car = std::make_shared<Car>(steering_motor, throtle_motor);

	return car;
}
