#pragma once

#include <firmata_client.hpp>

#include <cstdint>

class IotControl {

public:
	virtual void setPinMode(int32_t pin, int32_t mode) = 0;
	virtual void setPinHigh(int32_t pin) = 0;
	virtual void setPinLow(int32_t pin) = 0;
	virtual void setPWM(int32_t pin, int32_t value) = 0;
};

class FirmataIotControl : public IotControl {

public:
	FirmataIotControl(const std::string& port);

	void setPinMode(int32_t pin, int32_t mode);
	void setPinHigh(int32_t pin);
	void setPinLow(int32_t pin);
	void setPWM(int32_t pin, int32_t value);

private:
	FirmataClient firmataClient_;
};