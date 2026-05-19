#pragma once

#include "Graphics.hpp"
#include "Telemetry.hpp"

//Struct containing classes that glfw needs

//Pointers can be re-seated while references cannot. But we are not re seating anything, whatever
struct AllPointers 
{
	Camera* camera;
	/*Telemetry* telemetry;*/

	AllPointers(Camera* camera_/*, Telemetry* telemetry_*/)
		:camera(camera_)/*, telemetry(telemetry_)*/
	{}
};

//The standard is to use callbacks for one-time event (typing, increase something once per press) and another function
//  for per-frame polling that updates each frame or based on deltaTime for smooth changes in a variable over frames


inline void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{

	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;


	if (action == GLFW_PRESS)
	{
		//CTRL
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		{
			switch (key)
			{
			case GLFW_KEY_1:


				break;
			}

		}
		else
		{
			switch (key)
			{
			case GLFW_KEY_P:
				isRunning = !isRunning;
				print(isRunning);
				break;
				/*case GLFW_KEY_C:
					keyCounter++;

					break;*/
			case GLFW_KEY_Q:
				break;

				//cameraModes //updateCamera in camera->cpp also need to be updated if ths is to change
			case GLFW_KEY_X:
				camera->cameraMode = camera->drag;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				break;
			case GLFW_KEY_C:

				camera->cameraMode = camera->FPS;
				LastLMPos = mPos;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

				break;
			case GLFW_KEY_V:

				camera->cameraMode = camera->centered;
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


				break;

			}

		}

	}
}



//keys functions gets triggered once per frame
inline void keyboardRealTimePolls(GLFWwindow* window) 
{
	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;
	//The rest of the logic is in updateCamera
		//I THINK THIS SHOULD BE A CALL TO A CAMERA FUNCTION, BUT LOGIC SHOULD LIVE THERE
	// Rotation

	if (camera->cameraMode == camera->drag || camera->cameraMode == camera->FPS)
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			camera->calculateForward(camera->forward, camera->rotationSpeed, camera->right);

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			camera->calculateForward(camera->forward, -camera->rotationSpeed, camera->right);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			camera->calculateForward(camera->forward, -camera->rotationSpeed, camera->up);

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			camera->calculateForward(camera->forward, camera->rotationSpeed, camera->up);


		//translation
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera->cameraPos = camera->cameraPos + camera->forward * camera->translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera->cameraPos = camera->cameraPos - camera->forward * camera->translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera->cameraPos = camera->cameraPos - camera->right * camera->translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera->cameraPos = camera->cameraPos + camera->right * camera->translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera->cameraPos.y += camera->translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			camera->cameraPos.y -= camera->translationSpeed;
	}
	if (camera->cameraMode == camera->centered)
	{

	}



}



inline void getPos(GLFWwindow* window, p2& mPos) {
	double xpos1, ypos1;
	glfwGetCursorPos(window, &xpos1, &ypos1);

	mPos = { (float)xpos1,(float)(windowHeight - ypos1) };
}

inline void mouseEventCallback(GLFWwindow* window, int button, int action, int mods) {
	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;

	//LEFT
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		isLmbPressed = 1;

	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		isLmbPressed = 0;
	}

	//RIGHT
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{

	}


	//MIDDLE
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
		isMmbPressed = 1;
	}
	if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
	{
		isMmbPressed = 0;
	}
}


inline float scrollTranslationSpeedFactor = 20;

inline void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
	AllPointers* allPointers = static_cast<AllPointers*>(glfwGetWindowUserPointer(window));
	Camera* camera = allPointers->camera;


	if (yoffset > 0)
	{
		camera->cameraPos = camera->cameraPos + camera->forward * camera->translationSpeed * scrollTranslationSpeedFactor;

	}
	else if (yoffset < 0)
	{
		camera->cameraPos = camera->cameraPos - camera->forward * camera->translationSpeed * scrollTranslationSpeedFactor;

	}

}


