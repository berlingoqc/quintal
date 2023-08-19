#pragma once

#include <firmata_client.hpp>
#include <map>
#include <cstdint>

class IotControl {

public:
	virtual void setPinMode(int32_t pin, int32_t mode) = 0;
	virtual void setPinHigh(int32_t pin) = 0;
	virtual void setPinLow(int32_t pin) = 0;
	virtual void setPWM(int32_t pin, int32_t value) = 0;
};


class DumpIotControl : public IotControl {

public:
	DumpIotControl() {}

	void setPinMode(int32_t pin, int32_t mode) {}
	void setPinHigh(int32_t pin) {}
	void setPinLow(int32_t pin) {}
	void setPWM(int32_t pin, int32_t value) {}
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

	int32_t pinValue_[100] = {-1};
};