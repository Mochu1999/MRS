#include "SimpleFunctions.hpp"

float fastInverseSqrt(float number) {
	long i;
	float x2, y;
	x2 = number * 0.5F;
	y = number;
	std::memcpy(&i, &y, sizeof(i)); // Safer type-punning
	i = 0x5f3759df - (i >> 1);
	std::memcpy(&y, &i, sizeof(y)); // Safer type-punning
	y = y * (1.5f - (x2 * y * y));
	return y;
}

float radians(float input) {
	return input * 3.14159f * 0.00555;// inv180
}


float degrees(float input) {
	return input * 180 * 0.3183f; //invPI
}

string formatFloat(float value)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << value;
	return oss.str();
}