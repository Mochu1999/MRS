#pragma once

#include <chrono>
using namespace std::chrono;



//if isRunning = 0 the counter keeps summing, if you want to stop it create a variable pausedTime that only adds time if paused and is always been rest from the sum

struct TimeStruct 
{
	std::chrono::high_resolution_clock::time_point lastFrameTime;
	std::chrono::high_resolution_clock::time_point startElapsedTime;

	float currentTime = 0.0f;
	float  deltaTime = 0.0f;

	float fps = 0.0f;
	float frameCount = 0.0f;
	float timeAccumulator = 0.0f;

	//Transmitter
	const float transmitterUpdateInterval = 0.5f; //Doing a transmitter.update after this interval
	float transmitterUpdateAccumulator = 0;
	unsigned int counterUpdateTransmitter = 0;

	TimeStruct() 
	{
		lastFrameTime = std::chrono::high_resolution_clock::now();
		startElapsedTime = lastFrameTime;
	}

	void update() 
	{
		auto currentFrameTime = std::chrono::high_resolution_clock::now();

		currentTime = std::chrono::duration<float>(currentFrameTime - startElapsedTime).count();

		deltaTime = std::chrono::duration_cast<std::chrono::duration<float>>(currentFrameTime - lastFrameTime).count();
		lastFrameTime = currentFrameTime;


		updateFPS();
		updateTransmitter();
	}

	void updateFPS()
	{
		frameCount++;
		timeAccumulator += deltaTime;

		if (timeAccumulator >= 0.5f)
		{
			fps = frameCount / timeAccumulator;
			frameCount = 0;
			timeAccumulator -= 0.5f;
		}
	}

	void updateTransmitter()
	{
		transmitterUpdateAccumulator += deltaTime;

		while (transmitterUpdateAccumulator >= transmitterUpdateInterval)
		{
			transmitterUpdateAccumulator -= transmitterUpdateInterval;
			++counterUpdateTransmitter;
		}
	}
};


//To count elapsed time between whatever events. For debugging purposes
struct TimeCounter
{
	high_resolution_clock::time_point currentTime;
	high_resolution_clock::time_point lastTime;
	double endTime = std::numeric_limits<double>::max();

	TimeCounter()
	{
		currentTime = high_resolution_clock::now();
	}
	void endCounter() 
	{
		lastTime = high_resolution_clock::now();
		endTime = duration_cast<duration<double>>(lastTime - currentTime).count();
		std::cout << "Elapsed time: " << endTime << "s" << endl;
	}
};