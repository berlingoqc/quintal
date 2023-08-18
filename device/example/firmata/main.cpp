
#include <asio.hpp>
#include <iostream>
#include <vector>

#include <termios.h>
#include <unistd.h>

#include <firmata_client.hpp>

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
        FirmataClient client("/dev/pts/19");
        //client.getFirmwareInfo();

		enableRawMode();  // Enable raw mode to get keypresses immediately

        std::cout << "Press 'w' to set pin HIGH, release to set pin LOW. Press 'q' to quit." << std::endl;
        
        char ch;
        bool isWPressed = false;

		client.setPinMode(3, 3);
		client.setPinMode(4, 1);
		client.setPinMode(5, 1);

        client.setPWM(3, 1);
		
        client.setPinLow(4);
  		client.setPinHigh(5);

        while (true) {
            ch = std::cin.get();  // Read a character

            if (ch == 'w' && !isWPressed) {
				isWPressed = true;

                client.setPWM(3, 255);

            } else if (ch == 'w' && isWPressed) {
                isWPressed = false;
                client.setPWM(3, 128);
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