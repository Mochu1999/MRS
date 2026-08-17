#pragma once


// --- --- ---
//Constants that make use of custom types 
// --- --- ---
inline matrix4x4 identityMatrix = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };



// --- --- ---
//Global enums
// --- --- ---

enum ProgramState
{
	ship = 0,
	route = 1
};

enum FragmentMode
{
	shadeColor = 0,
	flatColor = 1
};



// --- --- ---
//Global variables
// --- --- ---
inline bool programState = route;

inline bool isRunning = true;

inline float windowHeight = 1080;
inline float windowWidth = 1920;
inline p2 windowTotal = { windowWidth,windowHeight };
inline p2 centerWindow = { windowWidth * 0.5f,windowHeight * 0.5f };

inline p2 mPos = { 0,0 };

////Removing variables from the struct to ease reference passings. To consider it for the final iteration
//It doesn't make sense to improve safety if every class is public
struct GlobalVariables
{

};