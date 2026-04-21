#pragma once

//Graphic functionality that doesn't deppend on Common (purely OpenGL and GLFW)

#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow* initialize();
GLFWwindow* initialize(float windowWidth_, float windowHeight_, const char* windowName = " ");


void getCursorPos(GLFWwindow* window);
void opaque();
void transparent();
void clearScreen();

