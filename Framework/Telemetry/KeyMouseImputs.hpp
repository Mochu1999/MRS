#pragma once





//Struct containing classes that glfw needs
//Mouse and keyboard inputs
struct InputGLFW
{
	GLFWwindow* window;
	Camera* camera;
	Telemetry* telemetry;

	InputGLFW(GLFWwindow* window_, Camera* camera_, Telemetry* telemetry_)
		:window(window_), camera(camera_), telemetry(telemetry_)
	{
		glfwSetWindowUserPointer(window, this); 

		glfwSetKeyCallback(window, keyboardEventCallback);
		glfwSetMouseButtonCallback(window, mouseEventCallback);
		glfwSetScrollCallback(window, scrollCallback);
	}

	static void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		InputGLFW* input =
			static_cast<InputGLFW*>(glfwGetWindowUserPointer(window));

		Camera* camera = input->camera;


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
	static void keyboardRealTimePolls(GLFWwindow* window)
	{
		InputGLFW* input =
			static_cast<InputGLFW*>(glfwGetWindowUserPointer(window));

		Camera* camera = input->camera;
		Telemetry* telemetry = input->telemetry;

		//The rest of the logic is in updateCamera
			//I THINK THIS SHOULD BE A CALL TO A CAMERA FUNCTION, BUT LOGIC SHOULD LIVE THERE
		// Rotation

		if (camera->cameraMode == camera->drag || camera->cameraMode == camera->FPS)
		{
			/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
				camera->calculateForward(camera->forward, camera->rotationSpeed, camera->right);

			if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
				camera->calculateForward(camera->forward, -camera->rotationSpeed, camera->right);

			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				camera->calculateForward(camera->forward, -camera->rotationSpeed, camera->up);

			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				camera->calculateForward(camera->forward, camera->rotationSpeed, camera->up);*/


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

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && telemetry->sailAngle >= -60)
			telemetry->sailAngle -= telemetry->sailIncrease;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && telemetry->sailAngle <= 60)
			telemetry->sailAngle += telemetry->sailIncrease;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && telemetry->rudderAngle >= -40)
			telemetry->rudderAngle -= telemetry->rudderIncrease;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && telemetry->rudderAngle <= 40)
			telemetry->rudderAngle += telemetry->rudderIncrease;

	}

	

	static void mouseEventCallback(GLFWwindow* window, int button, int action, int mods) {
		InputGLFW* input =
			static_cast<InputGLFW*>(glfwGetWindowUserPointer(window));

		Camera* camera = input->camera;

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


	

	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
		InputGLFW* input =
			static_cast<InputGLFW*>(glfwGetWindowUserPointer(window));

		Camera* camera = input->camera;
		float scrollTranslationSpeedFactor = 5;

		if (yoffset > 0)
		{
			camera->cameraPos = camera->cameraPos + camera->forward * camera->translationSpeed * scrollTranslationSpeedFactor;

		}
		else if (yoffset < 0)
		{
			camera->cameraPos = camera->cameraPos - camera->forward * camera->translationSpeed * scrollTranslationSpeedFactor;

		}

	}

	void getPos(GLFWwindow* window, p2& mPos) {
		double xpos1, ypos1;
		glfwGetCursorPos(window, &xpos1, &ypos1);

		mPos = { (float)xpos1,(float)(windowHeight - ypos1) };
	}
};