#pragma once

#include "motor.hpp"

class Car {

public:
	Car(DCMotor front, DCMotor back);

	void steerLeft(int32_t power);
	void steerRight(int32_t power);

	void forward(int32_t power);
	void backward(int32_t power);

	void stopSteer();
	void stopThrotle();

private:
	DCMotor front_;
	DCMotor back_;
};