#pragma once

#include "motor.hpp"

class Car {

public:
	Car(DCMotor front, DCMotor back);

	void steerLeft(int16_t power);
	void steerRight(int16_t power);

	void forward(int16_t power);
	void backward(int16_t power);

	void stopSteer();
	void stopThrotle();

private:
	DCMotor front_;
	DCMotor back_;
};