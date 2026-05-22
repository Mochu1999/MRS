#pragma once

//Sends bytes from a string
// In first iteration they are angle of rudder and angle of sail

struct TransmitterPC
{
	Telemetry t;

	std::string lastMessage;

	TransmitterPC(Telemetry& telemetry_)
		:t(telemetry_)
	{
		lastMessage = createMessage();

	}

	std::string createMessage()
	{
		std::ostringstream ss;
		ss << t.rudderAngle << " " << t.sailAngle;
		
		return ss.str();

	}

	void sendMessage()
	{
		//LoRa logic
	}
};