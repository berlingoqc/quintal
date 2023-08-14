
#include "iot_control.hpp"


FirmataIotControl::FirmataIotControl(const std::string& port) : firmataClient_(port) {}

void FirmataIotControl::setPinMode(int32_t pin, int32_t mode) {
	this->firmataClient_.setPinMode(pin, mode);
}

void FirmataIotControl::setPinHigh(int32_t pin) {
	this->firmataClient_.setPinHigh(pin);
}

void FirmataIotControl::setPinLow(int32_t pin) {
	this->firmataClient_.setPinLow(pin);
}

void FirmataIotControl::setPWM(int32_t pin, int32_t value) {
	this->firmataClient_.setPWM(pin, value);
}
