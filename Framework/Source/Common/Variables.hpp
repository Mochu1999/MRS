#pragma once
#include "Types.hpp"

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

//Custom variables
extern p2 cursor;
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