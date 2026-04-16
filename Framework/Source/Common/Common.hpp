#pragma once



using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
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
#include "SimpleFunctions.hpp"
#include "Types.hpp"
#include "Functions.hpp"

//not templated definitions of functions need to be separated from their declarations in .cpp

// Constants
constexpr double PI = 3.14159265358979323846;
constexpr double halfPI = PI / 2;
constexpr double invPI = 1.0f / PI;

constexpr float inv3 = 1.0f / 3.0f;
constexpr float inv180 = 1.0f / 180.0f;
constexpr float sqrt2 = 1.41421356237f;


constexpr float rho = 1025;
constexpr float g = 9.80665;
constexpr float eps = 1e-6f;



extern float windowHeight;
extern float windowWidth;
extern p2 windowTotal;
extern p2 windowCenter;
extern matrix4x4 identityMatrix;

enum Programs { telemetry, MRS, openCascade, offshoreProgram };
enum CameraModes { drag, FPS, centered }; //Por qué no está en camara?
enum VisualizationMode { wire, triangulated }; //OCC
enum MrsMode { mapMRS, mapCustom }; //1 MRS map, 2 custom map
enum CadMode { none, polyline, rectangle, circle, sphere, extrusion };

/////////////////////////////////////////////
struct GlobalVariables
{



	Programs program = telemetry;//to be changed in settings
	CameraModes cameraMode = centered;
	bool isRunning = true;
	p2 mPos = { 0,0 };

	bool isLmbPressed = 0;
	bool isMmbPressed = 0;
	p2 LastLMPos = { 0,0 };
	p2 LastMMPos = { 0,0 };
	p3 totalMiddleMPosVariation; //NO SÉ QUE ES
	p3 accumulativePositionChange; //LastMMPos measures mpos change, this is traspased to 3d with forward and right operations made on it

	p2 centerWindow;
	MrsMode mrsMode = mapMRS;//QUITAR


	VisualizationMode visualizationMode = triangulated;//QUITAR
	CadMode cadMode = none; //QUITAR
	vector <p3> cadPositions = { {0,0,0} };//QUITAR

	GlobalVariables()
	{
		centerWindow = { windowWidth / 2,windowHeight / 2 };
	}


};






void getCursorPos(GLFWwindow* window);








//Borrar
//aun con p chatgpt sigue diciendo que referencia podría no ser lo mejor. También sigue diciendo que un valor positivo es que p2 está a la derecha
template<typename T>
float crossProduct(const vec2<T>& p0, const vec2<T>& p1, const vec2<T>& p2) {
	vec2<T> v01 = { p1.x - p0.x, p1.y - p0.y };
	vec2<T> v12 = { p2.x - p1.x, p2.y - p1.y };

	return v01.x * v12.y - v01.y * v12.x;
}




//consts, cambiale el nombre y deja el nombre "is" para bools
float isRightOfLine(p2& A, p2& B, p2& P);



std::array<float, 4> createQuaternion(float angle, const p3& axis);

std::array<float, 4> inverseQuaternion(const std::array<float, 4>& q);

std::array<float, 4> multiplyQuaternions(const std::array<float, 4>& a, const std::array<float, 4>& b);

// q * p * q^-1
void rotatePoint(p3& point, const float& angle, const p3& axis);

void rotate3D(std::vector<p3>& vertices, const p3& centroid, float angleX, float angleY, float angleZ);
void rotate3D(std::vector<p3>& vertices, float angleX, float angleY, float angleZ);





float isBelowTriangle(const p3& a, const p3& b, const p3& c, const p3& p);



template<typename T>
vec3<T> centroid(const vec3<T>& p1, const vec3<T>& p2, const vec3<T>& p3) {
	return (p1 + p2 + p3) * inv3;
}

template<typename T>
vec3<T> normal(const vec3<T>& p1, const vec3<T>& p2, const vec3<T>& p3) {
	return normalize3(cross3(p2 - p1, p3 - p1));
}


std::array<float, 16> multiplyMatrices(const std::array<float, 16>& a, const std::array<float, 16>& b);
matrix4x4 invertMatrix(const matrix4x4& m);

std::array<float, 4> multiplyMatVec(const std::array<float, 16>& m, const std::array<float, 4>& v);

// Normalizes the quaternion [w, x, y, z] in-place
void normalizeQuaternion(std::array<float, 4>& q);


float radians(float input);


float degrees(float input);

string formatFloat(float value);

//Computes the intersection between 2 edges AB and CD
bool calculateIntersectionPoints(const p2 A, const p2 B, const p2 C, const p2 D, p2& i);

void opaque();
void transparent();

//Numerically Stable Quadratic Formula
void stableQuadraticSolver(float A, float B, float C, float& s0, float& s1);