
#include <boost/asio.hpp>
#include <iostream>
#include <vector>

#include <termios.h>
#include <unistd.h>

#include <firmata_client.hpp>

namespace asio = boost::asio;

void enableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);  // Disable canonical mode and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void disableRawMode() {
    struct termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag |= (ICANON | ECHO);  // Re-enable canonical mode and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}



int main() {
	try {
        FirmataClient client("/dev/ttyACM0");
        client.getFirmwareInfo();

		enableRawMode();  // Enable raw mode to get keypresses immediately

        std::cout << "Press 'w' to set pin HIGH, release to set pin LOW. Press 'q' to quit." << std::endl;
        
        char ch;
        bool isWPressed = false;

        client.setPWM(3, 16383);

        while (true) {
            ch = std::cin.get();  // Read a character

            if (ch == 'w' && !isWPressed) {
				isWPressed = true;
		        client.setPinHigh(4);
  		        client.setPinLow(5);

            } else if (ch == 'w' && isWPressed) {
                isWPressed = false;
		        client.setPinLow(4);
  		        client.setPinLow(5);
				std::cout << "stopping" << std::endl;
            } else if (ch == 'q') {
                break;
			}
            
        }

        disableRawMode();  // Restore terminal to normal mode
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
    return 0;
}