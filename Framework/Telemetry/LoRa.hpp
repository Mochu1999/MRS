#pragma once


/*
How to configure the LoRa module
Connect it to USB. Windows must recognize it as a serial port, not a device
In the device manager: ports, it may appear for example as USB-SERIAL CH340 (COM5)
If it isn't at COM5 change the variable portName
*/

//Sends bytes from a string
// On first iteration they are angle of rudder and angle of sail

using Clock = std::chrono::steady_clock; //TO BE UNIFIED

//Needed to find the CH340 port
#include <setupapi.h>
#include <devguid.h>
#pragma comment(lib, "setupapi.lib")

struct LoRa
{
	Telemetry& t;

	//Currently, name must be manually set
	string portName;
	//serial is Window's ID of the serial port
	HANDLE serial = INVALID_HANDLE_VALUE;

	string message; //it will update as soon as one of its parameters change
	bool isMessageNew = 0; //signals that a message must be sent to the ESP

	//Variables to be messaged
	//Currently these copies track if there are changes and the need to send a new message
	float lastRudderAngle;
	float lastSailAngle;


	//variables to control if a confirmation message has been received in the waitingConfirmationTime
	std::chrono::duration<long long, std::milli> waitingConfirmationTime = std::chrono::milliseconds(1500);
	bool isReceivingMssgPending = false;
	Clock::time_point lastSentMessageTime = Clock::now();

	//for debugging purposes
	int state = 0; //0-closed serial //1-sent message //2-confirmed message
	string lastMessage = "Closed serial!";


	LoRa(Telemetry& telemetry_)
		:t(telemetry_)
	{

		lastRudderAngle = t.rudderAngle;
		lastSailAngle = t.sailAngle;

		openSerial();

		message = createMessage();
		writeSerial();
	}

	~LoRa()
	{
		closeSerial();
	}

	//Checks if one of the telemetry values have changed, if so, it sends a message with the new values
	void update()
	{
		//Checking if variables have changed to modify message
		if (lastRudderAngle != t.rudderAngle || lastSailAngle != t.sailAngle)
		{
			isMessageNew = true;

			lastRudderAngle = t.rudderAngle;
			lastSailAngle = t.sailAngle;

			message = createMessage();

		}

		//updates counterUpdateTransmitter, will only write on the serial if enough time has elapsed
		t.tm.updateTransmitter();

		if (t.tm.counterUpdateTransmitter)
		{
			t.tm.counterUpdateTransmitter = 0; //resetting the counter

			//there's a message to be transmited
			if (isMessageNew)
			{
				//if it fails it will still try to send it
				if (writeSerial())
				{
					isMessageNew = 0;
				}
			}
		}


		
		//always sees if there's data available in the serial
		readSerial();




		

		//No confirmation message arrived, so we are sending the message again
		if (isReceivingMssgPending && Clock::now() - lastSentMessageTime >= waitingConfirmationTime)
		{
			isMessageNew = true;
			cout << "No confirmation mssg arrived. Sending mssg again\n";

			if (writeSerial())
			{
				isMessageNew = false;
			}
		}
		
	}


	std::string createMessage()
	{
		std::ostringstream ss;
		ss << round2d(t.rudderAngle) << " " << round2d(t.sailAngle) << "; ";
		return ss.str();
	}
	

	void openSerial()
	{
		portName = findLoRaPort();

		//In windows everything is treated like a file
		//A handle is windows internal reference to the opened resource, this rewrites it
		serial = CreateFileA(
			portName.c_str(),
			GENERIC_READ | GENERIC_WRITE, //Permission to read and write
			0, //do not allow other programs to access COM5 simultaneously
			nullptr, //default security configuration
			OPEN_EXISTING, //the device already exists
			0, //normal synchronous operation (blocks execution until Windows finishes handling the write request)
			nullptr //templates, meaningless for coms
		);

		//if it's still invalid it has failed to open the port
		if (serial == INVALID_HANDLE_VALUE)
		{
			std::cout << "Windows failed to open " << portName << " \n";
			return;
		}

		//Device Control Block
		//stores serial comunication settings
		DCB dcb = {};
		dcb.DCBlength = sizeof(dcb); //this dcb's struct variable must be set because GetCommState validates it this way before reading or writing its fields

		//Gets dcb settings in serial
		if (!GetCommState(serial, &dcb))
		{
			std::cout << "Couldn't get port setting of " << portName << " \n";

			CloseHandle(serial);
			serial = INVALID_HANDLE_VALUE;
			return;
		}

		//The following configuration creates 9600 8N1, the classic UART configuration
		//USB data is converted into UART electrical signals
		dcb.BaudRate = CBR_9600; //9600 bits/s
		dcb.ByteSize = 8; //8 bits per transmitted byte
		dcb.Parity = NOPARITY; //Parity is a primitive error detection mechanism.
		dcb.StopBits = ONESTOPBIT; //The stop bit marks the end of a byte. UART has no clock line. So the receiver needs a little idle time to resynchronize.
		dcb.fOutxCtsFlow = FALSE;
		dcb.fOutxDsrFlow = FALSE;
		dcb.fOutX = FALSE;
		dcb.fInX = FALSE;


		//Sends the new UART configuration into the device
		if (!SetCommState(serial, &dcb))
		{
			std::cout << "Couldn't set port " << portName << " \n";

			closeSerial();
			return;
		}

		//timeout configuration. Won't stop the program to wait for data
		// If there's data it will inmediately retrieve it with what's available
		COMMTIMEOUTS timeouts = {};

		timeouts.ReadIntervalTimeout = MAXDWORD;
		timeouts.ReadTotalTimeoutConstant = 0;
		timeouts.ReadTotalTimeoutMultiplier = 0;

		if (!SetCommTimeouts(serial, &timeouts))
		{
			std::cout << "Couldn't set serial timeouts\n";

			closeSerial();
			return;
		}

		std::cout << "Serial port opened\n";

	}

	void closeSerial()
	{
		if (serial != INVALID_HANDLE_VALUE)
		{
			CloseHandle(serial);
			serial = INVALID_HANDLE_VALUE;

			lastMessage = "Closed LoRa serial!";
			state = 0;
		}
	}


	//Opens port com if closed, configures it, sends bytes into port
	//true if the message is successfully sent, otherwise false
	bool writeSerial()
	{
		if (serial == INVALID_HANDLE_VALUE)
		{
			std::cout << "Serial port is not open\n";
			openSerial();

			//return if it wasn't open
			if (serial == INVALID_HANDLE_VALUE)
				return 0;
			else cout << "Opened port \n";
		}

		const char* mssg = message.c_str(); //needs raw bytes in c format

		//DWORD is the standard typedef windows uses for unsigned 32-bit integer
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

			closeSerial();
			return 0;
		}

		//else it has succeeded and starts waiting for a confirmation message
		lastSentMessageTime = Clock::now();
		isReceivingMssgPending = true;

		std::cout <<"Sending: "<< message << endl;

		

		//notifies if not all bytes have been sent
		if (bytesWritten / float(strlen(mssg)) != 1)
			std::cout << "Error sending the message. Percentage of successful written bytes: " << bytesWritten / float(strlen(mssg)) * 100 << "%\n";

		state = 1;
		lastMessage = "Sent:          " + message;
		return 1;

	}

	void readSerial()
	{
		if (serial == INVALID_HANDLE_VALUE)
		{
			std::cout << "Serial port is not open\n";
			openSerial();

			//return if it wasn't open
			if (serial == INVALID_HANDLE_VALUE)
				return;
			else cout << "Opened port \n";
		}

		char buffer[256];

		DWORD bytesRead = 0;

		BOOL received = ReadFile( serial, buffer, sizeof(buffer) - 1, &bytesRead, nullptr );

		if (!received)
		{
			std::cout << "Error reading from " << portName << "\n";
			closeSerial();
			return;
		}

		if (bytesRead > 0)
		{
			//adding a null terminator after the received bytes so buffer becomes a safe and valid Cstyle string
			buffer[bytesRead] = '\0';

			std::cout << "Received: " << buffer << "\n";

			processConfirmationMessage(buffer);
		}
	}

	void processConfirmationMessage(char buffer[256])
	{
		// ESP's c for confirmation must be the first character
		if (buffer[0] != 'c')
		{
			isMessageNew = true;
			cout << "incorrect ESP signature in confirmation mssg. Sending message again...\n";
			return;
		}

		float receivedRudderAngle;
		float receivedSailAngle;
		char endCharacter;

		// Skip the initial 'c'
		std::istringstream ss(buffer + 1);

		//checking that the format is correct
		if (!(ss >> receivedRudderAngle >> receivedSailAngle >> endCharacter) || endCharacter != ';')
		{
			isMessageNew = true;
			cout << "incorrect format in confirmation mssg. Sending message again...\n";

			return;
		}


		//comparison of the values
		//There might be floating errors in the comparison, using eps
		if (std::abs(receivedRudderAngle - t.rudderAngle) > 0.01 ||
			std::abs(receivedSailAngle - t.sailAngle) > 0.01)
		{
			isMessageNew = true;
			cout << "incorrect variables received from confirmation mssg. Sending message again...\n";

			return;
		}

		// All checks have failed so correct confirmation received
		isMessageNew = false;

		//confirmation received so we cancel the order of triggering a message because none arrived
		isReceivingMssgPending = false;

		state = 2;
		lastMessage = "Confirmed: " + message;

	}

	std::string findLoRaPort()
	{
		HDEVINFO devices = SetupDiGetClassDevs(
			&GUID_DEVCLASS_PORTS,
			nullptr,
			nullptr,
			DIGCF_PRESENT
		);

		SP_DEVINFO_DATA deviceInfo{};
		deviceInfo.cbSize = sizeof(deviceInfo);

		char name[256];

		for (DWORD i = 0; SetupDiEnumDeviceInfo(devices, i, &deviceInfo); i++)
		{
			if (SetupDiGetDeviceRegistryPropertyA(
				devices,
				&deviceInfo,
				SPDRP_FRIENDLYNAME,
				nullptr,
				reinterpret_cast<PBYTE>(name),
				sizeof(name),
				nullptr))
			{
				std::string deviceName = name;

				if (deviceName.find("CH340") != std::string::npos)
				{
					size_t start = deviceName.find("(COM");
					size_t end = deviceName.find(')', start);

					std::string com = deviceName.substr(start + 1, end - start - 1);

					SetupDiDestroyDeviceInfoList(devices);

					return "\\\\.\\" + com;
				}
			}
		}

		SetupDiDestroyDeviceInfoList(devices);
		return "";
	}
};
