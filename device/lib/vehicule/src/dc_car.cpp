#include "car.hpp"

Car::Car(DCMotor front, DCMotor back) : front_(front), back_(back) {

}

void Car::steerLeft(int32_t power) {
	this->front_.setPower(power);
	this->front_.forward();
}

void Car::steerRight(int32_t power) {
	this->front_.setPower(power);
	this->front_.backward();
}

void Car::forward(int32_t power) {
	this->back_.setPower(power);
	this->back_.forward();
}

void Car::backward(int32_t power) {
	this->back_.setPower(power);
	this->back_.backward();
}

void Car::stopSteer() {
	this->front_.stop();
}
void Car::stopThrotle() {
	this->back_.stop();
}
