#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include <string>

#include <cmath>
#include <numeric> 
#include <algorithm>

#include <array>
#include <vector>
#include <unordered_set>
#include <set>
#include <unordered_map>
#include <map>
#include <list>
#include <deque>

#include <functional>
#include <random>
#include <iomanip>

#include <filesystem>
namespace fs = std::filesystem;


//Functions that do not require any custom type from Types

#include <cstring>

// 1/sqrt(x)
float fastInverseSqrt(float number);

float radians(float input);
float degrees(float input);
string formatFloat(float value);