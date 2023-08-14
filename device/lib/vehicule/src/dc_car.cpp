#include "car.hpp"

Car::Car(DCMotor front, DCMotor back) : front_(front), back_(back) {

}

void Car::steerLeft(int16_t power) {
	this->back_.setPower(power);
	this->back_.forward();
}

void Car::steerRight(int16_t power) {
	this->back_.setPower(power);
	this->back_.backward();
}

void Car::forward(int16_t power) {
	this->front_.setPower(power);
	this->front_.forward();
}

void Car::backward(int16_t power) {
	this->front_.setPower(power);
	this->front_.backward();
}