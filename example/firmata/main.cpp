
#include <iostream>
#include <iomanip>

#include "serialport.hpp"
#include "firmata.hpp"


struct sysex_message {
	char id;
	int length;
	char payload[100];
};

class DataReceiver {
public:
    DataReceiver(SerialPortWriter& writer) : sp_writter(writer) {}

	SerialPortWriter& sp_writter; //(ioc, dev, braud_rate);
	
	std::string received_buffer_;

    void onDataReceived(const char* data, size_t length) {
		received_buffer_.append(data, length);

		int start_index = -1;
		int end_index = -1;
		for (int i = 0; i < received_buffer_.length(); i++) {
			if (received_buffer_[i] == FIRMATA_START_SYSEX) {
				start_index = i;
			}

			if (received_buffer_[i] == FIRMATA_END_SYSEX) {
				end_index = i;
			}
		}

		if (start_index > -1 && end_index > -1) {
			if (end_index > start_index) {
				// extract message and move buffer arround.
				auto sysex = sysex_message{ received_buffer_[start_index + 1], end_index - start_index - 3 };

				received_buffer_.copy(sysex.payload, sysex.length, 2);

				switch(sysex.id) {
					case FIRMATA_REPORT_FIRMWARE:
						std::cout << "REPORT FIRMWARE " << std::string(sysex.payload, sysex.length) << std::endl;
						
						{
							std::string payload(4, ' ');

							payload[0] = FIRMATA_START_SYSEX;
							payload[1] = FIRMATA_PIN_STATE_QUERY;
							payload[2] = 5;
							payload[3] = FIRMATA_END_SYSEX;

							sp_writter.writeData(payload);
						}

						break;
					case FIRMATA_PIN_STATE_RESPONSE:
						std::cout << "PIN STATE " << sysex.length << std::endl;
						{
							int pin = 4;
							int value = 1;

							int i = 0;
							int res = 0;
							std::string payload(4, ' ');

							int port_num = pin / 8;
							int port_value = 0;

							for (i = 0; i < 8; i++) {
								int p = port_num * 8 + i;

								port_value |= (1<<i);
							}

							payload[0] = FIRMATA_DIGITAL_MESSAGE | port_num;
							payload[1] = port_value & 0x7F;
							payload[2] = (port_value >> 7) & 0x7F;

							sp_writter.writeData(payload);

						}

						break;

					default:
						std::cout << "UNMAP " << std::hex << (0xFF & sysex.id) << " " << std::string(sysex.payload, sysex.length) << std::endl;
						break;
				}

			} else {
				std::cerr << "end index before start index" << std::endl;
			}
		}


    }
};

int main() {

    asio::io_context ioc;

	const auto dev = "/dev/ttyACM0";
	const auto braud_rate = 57600;

	SerialPortWriter sp_writter(ioc, dev, braud_rate);

	DataReceiver receiver(sp_writter);
	const auto callback = [&receiver](const char* data, size_t length) {
		receiver.onDataReceived(data, length);
	};

    SerialPortReader sp(make_strand(ioc), dev, braud_rate, callback);

    sp.read_async(SerialPortReader::ignore_timeout);

    ioc.run();

}