
#include "iot_control.hpp"


FirmataIotControl::FirmataIotControl(const std::string& port) : firmataClient_(port) {}

void FirmataIotControl::setPinMode(uint8_t pin, uint8_t mode) {
	this->firmataClient_.setPinMode(pin, mode);
}

void FirmataIotControl::setPinHigh(uint8_t pin) {
	this->firmataClient_.setPinHigh(pin);
}

void FirmataIotControl::setPinLow(uint8_t pin) {
	this->firmataClient_.setPinLow(pin);
}

void FirmataIotControl::setPWM(uint8_t pin, uint16_t value) {
	this->firmataClient_.setPWM(pin, value);
}
