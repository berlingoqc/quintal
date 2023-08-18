
#include "iot_control.hpp"


FirmataIotControl::FirmataIotControl(const std::string& port) : firmataClient_(port) {
        this->firmataClient_.getFirmwareInfo();
}

void FirmataIotControl::setPinMode(int32_t pin, int32_t mode) {
	if (this->pinValue_[pin] != mode) {
		this->firmataClient_.setPinMode(pin, mode);
		this->pinValue_[pin] = mode;
	}
}

void FirmataIotControl::setPinHigh(int32_t pin) {
	if (this->pinValue_[pin] != 1) {
		this->firmataClient_.setPinHigh(pin);
		this->pinValue_[pin] = 1;
	}
}

void FirmataIotControl::setPinLow(int32_t pin) {
	if (this->pinValue_[pin] != 0) {
		this->firmataClient_.setPinLow(pin);
		this->pinValue_[pin] = 0;
	}
}

void FirmataIotControl::setPWM(int32_t pin, int32_t value) {
	if (this->pinValue_[pin] != value) {
		this->firmataClient_.setPWM(pin, value);
		this->pinValue_[pin] = 1;
	}
}
