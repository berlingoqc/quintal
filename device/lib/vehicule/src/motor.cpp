
#include "motor.hpp"


DCMotor::DCMotor(
	std::shared_ptr<IotControl> control,
	int8_t int1,
	int8_t int2,
	int8_t pwm
) : control_(control) {
	this->control_->setPinMode(this->pin_int1_, 1);
	this->control_->setPinMode(this->pin_int2_, 1);
	this->control_->setPinMode(this->pin_pwm_, 1);
}

void DCMotor::setPower(int16_t power) {
	this->control_->setPWM(this->pin_pwm_, power);
}

void DCMotor::forward() {
	this->control_->setPinHigh(this->pin_int1_);
	this->control_->setPinLow(this->pin_int2_);
}

void DCMotor::backward() {
	this->control_->setPinLow(this->pin_int1_);
	this->control_->setPinHigh(this->pin_int2_);
}

void DCMotor::stop() {
	this->control_->setPinLow(this->pin_int1_);
	this->control_->setPinLow(this->pin_int2_);
}