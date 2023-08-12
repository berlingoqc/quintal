#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <vector>

namespace asio = boost::asio;

class FirmataClient {
public:
    FirmataClient(const std::string& port) 
        : io(), serial(io, port) {
        serial.set_option(asio::serial_port_base::baud_rate(57600));
    }

    void getFirmwareInfo() {
        // Send request
        sendSysex(0x79);

        // Receive response
        std::vector<uint8_t> response = receiveSysex();
        if (response.size() < 3 || response[0] != 0x79) {
            std::cerr << "Invalid response received!" << std::endl;
            return;
        }

        std::cout << "Firmware Name: ";
        for (size_t i = 1; i < response.size() - 1; i += 2) {
            std::cout << char(response[i]) << char(response[i + 1]);
        }
        std::cout << std::endl;
    }

    void setPWM(uint8_t pin, uint16_t value) {
        if (value > 0x3FFF) {
            std::cerr << "PWM value out of range. Maximum is 16383." << std::endl;
            return;
        }

        uint8_t command = 0xE0 | (pin & 0x0F);  // 0xE0 is the ANALOG_MESSAGE command
        writeByte(command | (value & 0x7F));
        writeByte(value >> 7);
    }

    void setPinMode(uint8_t pin, uint8_t mode) {
        writeByte(0xF4);
        writeByte(pin);
        writeByte(mode);
    }

	void setPinHigh(uint8_t pin) {
        setDigitalPinState(pin, 1);
    }

    void setPinLow(uint8_t pin) {
        setDigitalPinState(pin, 0);
    }

private:
    void sendSysex(uint8_t command) {
        writeByte(0xF0);  // Start of SysEx
        writeByte(command);
        writeByte(0xF7);  // End of SysEx
    }

    std::vector<uint8_t> receiveSysex() {
        std::vector<uint8_t> result;
        while (true) {
            uint8_t byte = readByte();
            if (byte == 0xF7) break;
            if (byte == 0xF0) continue;
            result.push_back(byte);
        }
        return result;
    }

    void writeByte(uint8_t byte) {
        asio::write(serial, asio::buffer(&byte, 1));
    }

    uint8_t readByte() {
        uint8_t byte;
        asio::read(serial, asio::buffer(&byte, 1));
        return byte;
    }

	void setDigitalPinState(uint8_t pin, uint8_t state) {
        uint8_t portNumber = pin / 8;  // Firmata uses 8-pin ports
        uint8_t portValue = 1 << (pin % 8);
        if (state == 0) {
            portValues[portNumber] &= ~portValue;  // Clear the bit for LOW
        } else {
            portValues[portNumber] |= portValue;  // Set the bit for HIGH
        }

        uint8_t command = 0x90 | portNumber;  // 0x90 is the DIGITAL_MESSAGE command
        writeByte(command);
        writeByte(portValues[portNumber] & 0x7F);  // Send 7 bits at a time
        writeByte(portValues[portNumber] >> 7);
    }

    uint8_t portValues[16] = {0};  

    asio::io_context io;
    asio::serial_port serial;
};