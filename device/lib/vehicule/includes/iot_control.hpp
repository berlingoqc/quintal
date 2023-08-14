#pragma once

#include <firmata_client.hpp>

#include <cstdint>

class IotControl {

public:
	virtual void setPinMode(uint8_t pin, uint8_t mode) = 0;
	virtual void setPinHigh(uint8_t pin) = 0;
	virtual void setPinLow(uint8_t pin) = 0;
	virtual void setPWM(uint8_t pin, uint16_t value) = 0;
};

class FirmataIotControl : public IotControl {

public:
	FirmataIotControl(const std::string& port);

	void setPinMode(uint8_t pin, uint8_t mode);
	void setPinHigh(uint8_t pin);
	void setPinLow(uint8_t pin);
	void setPWM(uint8_t pin, uint16_t value);

private:
	FirmataClient firmataClient_;
};