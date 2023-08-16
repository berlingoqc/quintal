#pragma once

#include <map>
#include <string>

#include "iot_control.hpp"
#include "car.hpp"


#include "config.pb.h"

class CarBuilder {

public:
	static std::map<std::string, std::shared_ptr<IotControl>> getControl(const google::protobuf::Map<std::string, IoTControlConfig>& config);

	CarBuilder(std::map<std::string, std::shared_ptr<IotControl>>& controls);


	std::shared_ptr<Car> buildCar(const CarConfig& config);

private:
	std::map<std::string, std::shared_ptr<IotControl>> controls_;
};