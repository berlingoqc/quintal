#pragma once

#include "iot_control.hpp"

class DCMotor {

public:
	DCMotor(
		std::shared_ptr<IotControl> control,
		int8_t int1,
		int8_t int2,
		int8_t pwm
	);

	void setPower(int16_t power);
	void forward();
	void backward();
	void stop();

private:
	std::shared_ptr<IotControl> control_;

	int8_t pin_int1_;
	int8_t pin_int2_;
	int8_t pin_pwm_;
};