#pragma once

//Header containing the low level logic for the Graphics Engine


#include "Common.hpp"



//Access to OpenGL and GLFW
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

//Logic that only involves OpengGL and GLFW
#include "SimpleGraphics.hpp"




//Foundational logic of the Graphics Engine
//Both Camera and Shader are at the same level and do not depend of each other
#include "Shader.hpp"
#include "Camera.hpp"

//Initializes OpenGL locations of the Camera
void initializeCameraLocations(Shader& shader3D, Shader& shader2D, Shader& shader2D_Instanced, Shader& shaderText, Shader& shaderText3D, Shader& shaderWater, Camera& camera);
//updating view camera shader location
void updateCameraLocations(Shader& shader3D, Shader& shaderText3D, Shader& shaderWater, Camera& camera);


//Basic Rendering
#include "DelaunayTriangulation.hpp"

#include "RectanglesCircles.hpp"
#include "Lines2D.hpp"
#include "Lines2DInstanced.hpp"
#include "Lines3D.hpp"


#include "Sphere.hpp"

#include "Polygons2D.hpp"
#include "Polygons3D.hpp"

#include "FilesManagement.hpp" //from Core
#include "Polyhedra.hpp"
void polyhedraToPolygon2D(Polyhedra polyhedra, Polygons2D& polygon);

#include "Text.hpp"
#include "Text3D.hpp"

#include "SweepTriangulation.hpp"