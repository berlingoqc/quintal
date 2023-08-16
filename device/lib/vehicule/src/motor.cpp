
#include "motor.hpp"


DCMotor::DCMotor(
	std::shared_ptr<IotControl> control,
	int32_t int1,
	int32_t int2,
	int32_t pwm
) : control_(control), pin_int1_(int1), pin_int2_(int2), pin_pwm_(pwm) {
	this->control_->setPinMode(this->pin_int1_, 1);
	this->control_->setPinMode(this->pin_int2_, 1);
	this->control_->setPinMode(this->pin_pwm_, 3);

	std::cout << "dm motor initialize " << this->pin_int1_ << " " << this->pin_int2_ << " pwm " << this->pin_pwm_ << std::endl;
}

void DCMotor::setPower(int32_t power) {
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