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
inline bool programState = ship;

inline bool isRunning = true;

inline float windowHeight = 1080;
inline float windowWidth = 1920;
inline p2 windowTotal = { windowWidth,windowHeight };
inline p2 centerWindow = { windowWidth * 0.5f,windowHeight * 0.5f };

inline p2 mPos = { 0,0 };


//colors
inline p3 white = { 1,1,1 };
inline p3 almostWhite = { 0.9,0.9,0.9 }; //to better appreciate shades
inline p3 deepGrey = { 0.1, 0.1, 0.1 };
inline p3 garnet = { 137.0f / 255.0f, 18.0f / 255.0f, 18.0f / 255.0f };
inline p3 lightBlue = { 40.0f / 255.0f, 189.9f / 255.0f, 255.0f / 255.0f };

inline p3 red = { 1,0,0 };
inline p3 green = { 0,1,0 };
inline p3 blue = { 0,0,1 };


////Removing variables from the struct to ease reference passings. To consider it for the final iteration
//It doesn't make sense to improve safety if every class is public
struct GlobalVariables
{

};