#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <cstring>

#include <fcntl.h> // open
#include <unistd.h> // read, write, close
#include <termios.h>// termios
#include <errno.h>

#include <chrono>
using Clock = std::chrono::steady_clock; //intended for measuring elapsed time


struct LoRa
{
    Parameters& p;

    //const char* portName = "/dev/ttyUSB1"; //run ls /dev/ttyUSB* in linux ps to check if it is at 0
    const char* portName = "/dev/serial/by-id/usb-1a86_USB_Serial-if00-port0"; //got it from the command ls -l /dev/serial/by-id/

    //The Linux serial handle to the opened port. More precisely a file descriptor to open, clore, read and write
    int serial = -1;
    bool isSerialSet = false;

    //linux equivalent of DCB dcb{};, that is the serial/uart configuration
    termios tty{};

    //The serial is expected to be read in the C char
    //space for 256 bytes
    char buffer[256];
    std::string latestMessage;
    

    //It will wait for 1.5 seconds to send the message back once a message arrives
    std::chrono::duration<long long, std::milli> waitingSendBackTime = std::chrono::milliseconds(1500);
    bool isSendBackPending = false;
    Clock::time_point lastMessageTime = Clock::now();


    LoRa(Parameters& p_)
        :p(p_)
    {
        openserial();
    }

    ~LoRa()
    {
        closeSerial();
    }

    void update()
    {
        if (!isSerialSet)
            openserial();

        if (isSerialSet)
        {
            //Always trying to read
            readSerial();
            //Will only write if isSendBackPending==true and enough time has elapsed
            writeSerial();
        }
    }

    void openserial()
    {
        serial = open(portName, O_RDWR | O_NOCTTY);
        //O_RDWR: open for reading and writing
        //O_NOCTTY: prevents the terminal from becoming the controlling terminal for the process

        //even if it correctly opens, we don't set isSerialSet until the configuration is set
        if (serial < 0)
        {
            std::cout << "Error: could not open " << portName << "\n";
            std::cout << "error: " << strerror(errno) << "\n"; //converts the error into readable text
            return;
        }

        if (tcgetattr(serial, &tty) != 0)
        {
            std::cout << "Error: could not get LoRa serial settings\n";
            close(serial);
            return;
        }

        //setting uart configuration
        {
            //sets baud rate (communication speed)
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
        }

        //applies the configuration into the serial
        if (tcsetattr(serial, TCSANOW, &tty) != 0)
        {
            std::cout << "Error: could not apply LoRa serial settings\n";
            close(serial);
            return;
        }

        std::cout << "Listening on " << portName << "...\n";

        isSerialSet = true;
    }

    //safely closing it if bytesRead/Written is assigned an invalid value. It will be reopened in next loop iteration
    void closeSerial()
    {
        if (serial >= 0)
        {
            close(serial);
            serial = -1;
        }

        isSerialSet = false;
    }

    void readSerial()
    {
        //number of bytes received if >0, if <0 is an error, if 0 no bytes received before timeout
        //reading empties the serial buffer (and loads it in buffer)
        ssize_t  bytesRead = read(serial, buffer, sizeof(buffer));

        if (bytesRead < 0)
        {
            std::cout << "Error while reading LoRa\n";
            closeSerial();
            return;
        }

        if (bytesRead > 0)
        {
            // Store latest received message
            latestMessage.assign(buffer, bytesRead);

            //std::cout << "Received " << bytesRead << " bytes: ";
            std::cout << "Received message: " << latestMessage << "\n";

            // A new message has arrived, so it restarts the 1.5 s timer
            lastMessageTime = Clock::now();

            isSendBackPending = true;

            processMessage();
        }
    }

    void writeSerial()
    {
        // If waitingSendBackTime has elapsed
        if (isSendBackPending && Clock::now() - lastMessageTime >= waitingSendBackTime)
        {

            //we add a "r" so there's no misunderstanding on the windows side that the package comes from the raspi
            std::string response = "r" + latestMessage;
            ssize_t  bytesWritten = write(serial, response.data(), response.size());

            if (bytesWritten < 0)
            {
                std::cout << "Error while writing LoRa: " << std::strerror(errno) << '\n';
                closeSerial();
                return;
            }
            else
            {
                std::cout << "Sent back: " << response << "\n";

                isSendBackPending = false;
            }
        }
    }

    //if the format isn't correct or it does not end with a ";", then it won't update the parameters
    // If that happens confirmation mssg should be messed up and another should be sent from the pc
    void processMessage()
    {
        float receivedRudderAngle;
        float receivedSailAngle;
        char endCharacter;

        std::istringstream ss(latestMessage);

        //current message format
        if (!(ss >> receivedRudderAngle >> receivedSailAngle >> endCharacter))
            return;

        if (endCharacter != ';')
            return;

        p.rudderAngle = receivedRudderAngle;
        p.sailAngle = receivedSailAngle;
        //std::cout << "processed message: " << p.rudderAngle << ", " << p.sailAngle << std::endl;
    }
};