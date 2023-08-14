#pragma once

#include "iot_control.hpp"

class DCMotor {

public:
	DCMotor(
		std::shared_ptr<IotControl> control,
		int32_t int1,
		int32_t int2,
		int32_t pwm
	);

	void setPower(int32_t power);
	void forward();
	void backward();
	void stop();

private:
	std::shared_ptr<IotControl> control_;

	int32_t pin_int1_;
	int32_t pin_int2_;
	int32_t pin_pwm_;
};