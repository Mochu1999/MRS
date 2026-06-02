#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <string>
int main(void)
{

    const char* portName = "\\\\.\\COM5";

    HANDLE serial = CreateFileA(
        portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        0,
        nullptr
    );

    if (serial == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Error: could not open COM5\n";
        return 1;
    }

    DCB dcb = {};
    dcb.DCBlength = sizeof(dcb);

    if (!GetCommState(serial, &dcb))
    {
        std::cerr << "Error: could not get COM state\n";
        CloseHandle(serial);
        return 1;
    }

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    if (!SetCommState(serial, &dcb))
    {
        std::cerr << "Error: could not configure COM5\n";
        CloseHandle(serial);
        return 1;
    }

    COMMTIMEOUTS timeouts = {};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;

    SetCommTimeouts(serial, &timeouts);

    std::cout << "Listening on COM5...\n";

    while (true)
    {
        char buffer[256];
        DWORD bytesRead = 0;

        BOOL ok = ReadFile(
            serial,
            buffer,
            sizeof(buffer) - 1,
            &bytesRead,
            nullptr
        );

        if (!ok)
        {
            std::cerr << "Error while reading from COM5\n";
            break;
        }

        if (bytesRead > 0)
        {
            buffer[bytesRead] = '\0';
            std::cout << buffer << std::flush;
        }

        Sleep(10);
    }

    CloseHandle(serial);
    return 0;
}