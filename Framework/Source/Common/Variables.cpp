#include "Variables.hpp"

//Variables
p2 cursor = { 0, 0 };
float windowHeight = 1080;
float windowWidth = 1920;
p2 windowTotal = { windowWidth,windowHeight };
p2 windowCenter = { windowWidth * 0.5f, windowHeight * 0.5f };
matrix4x4 identityMatrix = { 1, 0, 0, 0, 0, 1, 0, 0,0, 0, 1, 0, 0, 0, 0, 1 };