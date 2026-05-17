#pragma once

//Graphic functionality that doesn't deppend on Common (purely OpenGL and GLFW)

GLFWwindow* initialize();
GLFWwindow* initialize(float windowWidth_, float windowHeight_, const char* windowName = " ");

void glfwIput(GLFWwindow* window, void* pointer, GLFWkeyfun callbackKey, GLFWmousebuttonfun callbackMouseButton, GLFWscrollfun callbackScroll);

void getCursorPos(GLFWwindow* window);
void opaque();
void transparent();
void clearScreen();

//glLineWidth(2);

