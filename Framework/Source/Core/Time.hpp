#pragma once

#include <chrono>
using namespace std::chrono;

#include "Common.hpp"

//Tener todos los relojes de una aplicación compleja por separado no es viable. Estudiar meter la funcionalidad en common y implementar dentro de las que sean necesario

struct TimeStruct 
{
	std::chrono::high_resolution_clock::time_point lastFrameTime;
	std::chrono::high_resolution_clock::time_point startElapsedTime;

	float currentTime = 0.0f;
	float  deltaTime = 0.0f;

	//these 2 variables calculate the fps counter over a small period of time
	//they get reset over that small period of time. The fps variable is on FpsCounter.hpp
	float fpsTimeAccumulator = 0.0f;
	float frameCount = 0.0f;


	//Transmitter
	const float transmitterUpdateInterval = 0.7f; //Doing a transmitter.update after this interval
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

	//updates the 2 variables that change over time
	void updateFPS()
	{
		fpsTimeAccumulator += deltaTime;
		frameCount++;
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
		std::cout << "Elapsed time: " << endTime << "s" << std::endl;
	}
};