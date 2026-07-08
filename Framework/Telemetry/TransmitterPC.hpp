#pragma once

//Sends bytes from a string
// In first iteration they are angle of rudder and angle of sail

/*
Configure module
Connect it to USB. Windows must recognize it as a serial port, not a device
In the device manager: ports, it may appear for example as: USB-SERIAL CH340 (COM5)
*/

//only one copy per struct or the handle will break
struct TransmitterPC
{
	Telemetry& t;

	//Change name if the port has been changed
	const char* portName = "\\\\.\\COM5";
	HANDLE serial = INVALID_HANDLE_VALUE;

	float lastRudderAngle;
	float lastSailAngle;

	TransmitterPC(Telemetry& telemetry_)
		:t(telemetry_)
	{
		lastRudderAngle = t.rudderAngle;
		lastSailAngle = t.sailAngle;

		openSerial();

		string message = createMessage();
		sendMessage(message);
	}

	~TransmitterPC()
	{
		if (serial != INVALID_HANDLE_VALUE)
		{
			CloseHandle(serial);
			serial = INVALID_HANDLE_VALUE;
		}
	}

	//Checks if one of the telemetry values have changed, if so, it sends a message with the new values
	void update()
	{
		t.tm.updateTransmitter();

		//So transmitter cannot be spammed
		if (t.tm.counterUpdateTransmitter)
		{
			t.tm.counterUpdateTransmitter = 0; //resetting the counter

			if (lastRudderAngle != t.rudderAngle || lastSailAngle != t.sailAngle)
			{
				lastRudderAngle = t.rudderAngle;
				lastSailAngle = t.sailAngle;

				string message = createMessage();
				sendMessage(message);
				//print(message);
			}
		}
	}

	std::string createMessage()
	{
		std::ostringstream ss;
		ss << t.rudderAngle << " " << t.sailAngle << "; ";
		return ss.str();
	}


	void openSerial()
	{

		//In windows everything is treated like a file
		//A handle is windows internal reference to the opened resource
		serial = CreateFileA(
			portName,
			GENERIC_READ | GENERIC_WRITE, //Permission to read and write
			0, //do not allow other programs to access COM5 simultaneously
			nullptr, //default security configuration
			OPEN_EXISTING, //the device already exists
			0, //normal synchronous operation (blocks execution until Windows finishes handling the write request)
			nullptr //templates, meaningless for coms
		);
		if (serial == INVALID_HANDLE_VALUE)
		{
			std::cout << "Windows failed to open " << portName << " \n";
			return;
		}

		//Device Control Block
		//stores serial comunication settings
		DCB dcb = {};
		dcb.DCBlength = sizeof(dcb); //Old WinAPI APIs commonly require this.

		//Gets dcb settings in serial
		if (!GetCommState(serial, &dcb))
		{
			std::cout << "Couldn't get port " << portName << " \n";

			CloseHandle(serial);
			serial = INVALID_HANDLE_VALUE;
			return;
		}

		//The following configuration creates 9600 8N1, the classic UART configuration
		dcb.BaudRate = CBR_9600; //9600 bits/s
		dcb.ByteSize = 8; //8 bits per transmitted byte
		dcb.Parity = NOPARITY; //Parity is a primitive error detection mechanism.
		dcb.StopBits = ONESTOPBIT; //The stop bit marks the end of a byte. UART has no clock line. So the receiver needs a little idle time to resynchronize.


		//Sends the new UART configuration into the device
		if (!SetCommState(serial, &dcb))
		{
			std::cout << "Couldn't set port " << portName << " \n";

			CloseHandle(serial);
			serial = INVALID_HANDLE_VALUE;
			return;
		}
	}



	//Opens port com,configures it, sends bytes into port, closes it
	void sendMessage(string& message)
	{
		if (serial == INVALID_HANDLE_VALUE)
		{
			std::cout << "Serial port is not open\n";
			openSerial();

			//has it oppened yet?
			if (serial == INVALID_HANDLE_VALUE)
				return;
		}


		const char* mssg = message.c_str(); //needs raw bytes in c format


		//DWORD is the typedef windows uses for unsigned 32-bit integer
		//windows writes back how many bytes succeeded. But it doesn't guarantee data was transmitted
		DWORD bytesWritten = 0;

		//sending bytes into port
		BOOL sendMssg = WriteFile(
			serial,
			mssg,
			static_cast<DWORD>(strlen(mssg)), //number of bytes of the mssg
			&bytesWritten,
			nullptr //WriteFile() blocks until the operation is handled
		);

		if (!sendMssg)
		{
			std::cout << "Error writing in port " << portName << " \n";

			return;
		}

		//percentage of bytes that have succeeden
		std::cout << "Sent message at " << 0 << ". Percentage of succeeded bytes: " << bytesWritten / float(strlen(mssg)) * 100 << "%\n";


	}
};

//receiver
/*
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
*/