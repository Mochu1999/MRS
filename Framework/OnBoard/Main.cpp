#include <iostream>
#include <string>
#include <cstring>

#include <fcntl.h>      // open
#include <unistd.h>     // read, write, close
#include <termios.h>    // termios
#include <errno.h>

int main()
{
    const char* portName = "/dev/ttyUSB0";

    int serial = open(portName, O_RDWR | O_NOCTTY);

    if (serial < 0)
    {
        std::cerr << "Error: could not open " << portName << "\n";
        std::cerr << "errno: " << strerror(errno) << "\n";
        return 1;
    }

    termios tty{};

    if (tcgetattr(serial, &tty) != 0)
    {
        std::cerr << "Error: could not get serial settings\n";
        close(serial);
        return 1;
    }

    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // 8 data bits
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    // no parity
    tty.c_cflag &= ~PARENB;

    // 1 stop bit
    tty.c_cflag &= ~CSTOPB;

    // disable hardware flow control
    tty.c_cflag &= ~CRTSCTS;

    // enable receiver, ignore modem control lines
    tty.c_cflag |= CREAD | CLOCAL;

    // raw mode: no line processing
    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    // read timeout behavior
    tty.c_cc[VMIN] = 0;
    tty.c_cc[VTIME] = 1; // 0.1 seconds

    if (tcsetattr(serial, TCSANOW, &tty) != 0)
    {
        std::cerr << "Error: could not apply serial settings\n";
        close(serial);
        return 1;
    }

    std::cout << "Listening on " << portName << "...\n";

    while (true)
    {
        char buffer[256];

        int bytesRead = read(serial, buffer, sizeof(buffer));

        if (bytesRead < 0)
        {
            std::cerr << "Error while reading\n";
            break;
        }

        if (bytesRead > 0)
        {
            std::cout << "Received " << bytesRead << " bytes: ";
            std::cout.write(buffer, bytesRead);
            std::cout << "\n";

            int bytesWritten = write(serial, buffer, bytesRead);

            if (bytesWritten < 0)
            {
                std::cerr << "Error while writing echo response\n";
                break;
            }

            std::cout << "Echoed back " << bytesWritten << " bytes\n";
        }

        usleep(10000); // 10 ms
    }

    close(serial);
    return 0;
}