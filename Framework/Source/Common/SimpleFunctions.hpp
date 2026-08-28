#pragma once

//Functions that do not require any custom type from Types

//Making use of inline to not force the use of a .cpp for small definitions

// 1/sqrt(x)
inline float fastInverseSqrt(float number) {
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

inline float radians(float input) {
	return input * PI * inv180;
}


inline float degrees(float input) {
	return input * 180 * invPI;
}

inline float normalizeAngleDegrees(float angle)
{
	while (angle < 0)
		angle += 360;
	while (angle >= 360)
		angle -= 360;

	return angle;
}

inline std::string formatFloat(float value)
{
	std::ostringstream oss;
	oss << std::fixed << std::setprecision(2) << value;
	return oss.str();
}


inline float knToMs(float kn)
{
	return kn * 0.5144444;
}

inline float msToKn(float ms)
{
	return ms * 1.943844;
}