#pragma once

//In Common we find general includes, constants that don't make use of custom types
// , and the includes of custom headers: SimpleFunctions, Types, Variables and Functions


//Setting here the most used C++ backend includes for easier iteration. The idiomatic choice would be to include just what's needed in each .cpp

#include <iostream>
using namespace std;
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

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



// Constants
constexpr double PI = 3.14159265358979323846;
constexpr double halfPI = PI / 2;
constexpr double invPI = 1.0 / PI;

constexpr double inv3 = 1.0 / 3.0;
constexpr double inv180 = 1.0 / 180.0;
constexpr double sqrt2 = 1.41421356237;

constexpr double rho = 1025.0;
constexpr double g = 9.80665;
constexpr double eps = 1e-6;



//Custom files
#include "SimpleFunctions.hpp"
#include "Types.hpp"
#include "Variables.hpp"
#include "Functions.hpp"











