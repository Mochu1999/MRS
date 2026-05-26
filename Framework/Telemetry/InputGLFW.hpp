#pragma once

//Struct containing classes that glfw needs
//Mouse and keyboard inputs
struct InputGLFW
{
	GLFWwindow* window;
	Camera* camera;
	Telemetry* telemetry;
	Buttons* buttons;

	//speed and angles that change the camera angle on input
	float translationSpeed = 0.05f, rotationSpeed = 0.002f;
	float centeredAngleRotation = 0.1;

	bool isLMBPressed = 0;
	bool isMMBPressed = 0;

	p2 lastLMBPos, lastMMBPos;
	p2 lastMPos;

	p2 lMBPosVariation, mMBPosVariation;
	p2 mPosVariation;

	p2 lastDragMPos;

	bool isDraggingWindow = false;


	InputGLFW(GLFWwindow* window_, Camera* camera_, Telemetry* telemetry_, Buttons* buttons_)
		:window(window_), camera(camera_), telemetry(telemetry_), buttons(buttons_)
	{
		glfwSetWindowUserPointer(window, this); //Stores a pointer to this specific InputGLFW instance inside the GLFWwindow

		glfwSetKeyCallback(window, keyboardEventCallback);
		glfwSetMouseButtonCallback(window, mouseEventCallback);
		glfwSetScrollCallback(window, scrollCallback);
	}

	//customPolls implicitly has the "this" pointer available, as it is a normal member function
	//so camera and telemetry instances like camera->cameraPos are actually this->camera->cameraPos
	//but that function is the only one that isn't a GLFW callback
	//GLFW callback functions expect a function pointer, meaning it must have those parameters and nothing more.
	//And standard non-static member functions need a "this" (all of them, even if it isn't specified)
	//Static on a member function means “This function belongs to the class itself, not to an object instance” (has no "this")
	//having no "this" matches what GLFW expects for a C-style callback function
	//The "self" references in the functions are a reconstruction of "this", a pointer of the InputGLFW struct object

	//A call each frame
	void customPolls(GLFWwindow* window)
	{
		//--- --- ---
		// Mouse movement 
		//--- --- ---


		mPosVariation = lastMPos - mPos;
		lMBPosVariation = lastLMBPos - mPos;

		mMBPosVariation = lastMMBPos - mPos;
		//In drag, when the left mouse button stays pressed, forward rotates with lMBPosVariation
		if (camera->cameraMode == camera->drag && isLMBPressed)
		{
			//horizontal and vertical rotations
			camera->rotateForward(-lMBPosVariation.x * rotationSpeed, camera->up);
			camera->rotateForward(lMBPosVariation.y * rotationSpeed, camera->right);

			lastLMBPos = mPos;
		}
		//also rotates with lMBPosVariation in FPS, but you don't need to press to rotate
		else if (camera->cameraMode == camera->FPS)
		{
			camera->rotateForward(mPosVariation.x * rotationSpeed, camera->up);
			camera->rotateForward(-mPosVariation.y * rotationSpeed, camera->right);

			lastMPos = mPos;
		}

		//centered and drag MMB translation
		if (isMMBPressed && camera->cameraMode == camera->drag)
		{
			//camera->cameraPos = { 10,10,0 };
			camera->cameraPos += mMBPosVariation.x * camera->right * 0.002;

			camera->cameraPos += mMBPosVariation.y * camera->up * 0.002;
			lastMMBPos = mPos;

		}
		//window translation
		if (isDraggingWindow)
		{
			int winX, winY;
			glfwGetWindowPos(window, &winX, &winY);

			// cursor in screen coords
			int screenCursorX = winX + (int)mPos.x;
			int screenCursorY = winY + (int)mPos.y;

			//move window so the cursor stays at the same grab offset inside the window
			int newX = screenCursorX - (int)dragStartCursorX;
			int newY = screenCursorY - (int)dragStartCursorY;

			//glfwSetWindowPos(window, newX, newY);
		}


		//--- --- ---
		// Keyboard 
		//--- --- ---
		if (camera->cameraMode == camera->drag || camera->cameraMode == camera->FPS)
		{

			//Moves forward with arrows
			/*{
				if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
					camera->rotateForward(rotationSpeed, camera->right);

				if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
					camera->rotateForward(-rotationSpeed, camera->right);

				if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
					camera->rotateForward(-rotationSpeed, camera->up);

				if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
					camera->rotateForward(rotationSpeed, camera->up);
			}*/


			//translation
			// In drag and FPS it just translates
			{
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
					camera->cameraPos = camera->cameraPos + camera->forward * translationSpeed;

				if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
					camera->cameraPos = camera->cameraPos - camera->forward * translationSpeed;

				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
					camera->cameraPos = camera->cameraPos - camera->right * translationSpeed;

				if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
					camera->cameraPos = camera->cameraPos + camera->right * translationSpeed;

				if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
					camera->cameraPos.y += translationSpeed;

				if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
					camera->cameraPos.y -= translationSpeed;
			}

		}
		//Centered translation
		// wasd rotates cameraPos around a point and recalculates forward
		if (camera->cameraMode == camera->centered)
		{
			//rotates camera around center
			{
				if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				{
					rotatePoint(camera->cameraPos, centeredAngleRotation, p3{ 0, 1, 0 });
				}
				if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				{
					rotatePoint(camera->cameraPos, -centeredAngleRotation, p3{ 0, 1, 0 });
				}
				if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				{
					rotatePoint(camera->cameraPos, -centeredAngleRotation, camera->right);

					//it was rotated too close to top limit, so undoes it
					//This logic must necessarily be wrong. And yet it doesn't break
					if (camera->up.y <= 2 * centeredAngleRotation && camera->cameraPos.y > 0)
						rotatePoint(camera->cameraPos, centeredAngleRotation, camera->right);
				}
				if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				{
					rotatePoint(camera->cameraPos, centeredAngleRotation, camera->right);

					if (camera->up.y <= 2 * centeredAngleRotation && camera->cameraPos.y < 0)
						rotatePoint(camera->cameraPos, -centeredAngleRotation, camera->right);
				}
				camera->forward = normalize3(-camera->cameraPos);
				camera->right = normalize3(cross3(camera->forward, { 0,1,0 }));
				camera->up = cross3(camera->right, camera->forward);
			}
		}

		//--- --- ---
		// Telemetry keyboard
		//--- --- ---
		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS && telemetry->sailAngle >= -60)
			telemetry->sailAngle -= telemetry->sailIncrease;
		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS && telemetry->sailAngle <= 60)
			telemetry->sailAngle += telemetry->sailIncrease;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && telemetry->rudderAngle >= -40)
			telemetry->rudderAngle -= telemetry->rudderIncrease;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && telemetry->rudderAngle <= 40)
			telemetry->rudderAngle += telemetry->rudderIncrease;

	}


	//Only one poll per press
	static void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		InputGLFW* self = static_cast<InputGLFW*>(glfwGetWindowUserPointer(window));

		Camera* camera = self->camera;


		if (action == GLFW_PRESS)
		{
			//CTRL
			if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			{
				switch (key)
				{
				case GLFW_KEY_P:
					isRunning = !isRunning;
					print(isRunning);
					break;
				case GLFW_KEY_1:


					break;
				}

			}
			else
			{
				switch (key)
				{

					/*case GLFW_KEY_C:
						keyCounter++;

						break;*/
				case GLFW_KEY_Q:
					break;

				case GLFW_KEY_X:
					camera->cameraMode = camera->drag;
					glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
					break;
				case GLFW_KEY_C:

					camera->cameraMode = camera->FPS;
					self->lastMPos = mPos;
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


	static void mouseEventCallback(GLFWwindow* window, int button, int action, int mods) {
		InputGLFW* self = static_cast<InputGLFW*>(glfwGetWindowUserPointer(window));

		Camera* camera = self->camera;
		Buttons* buttons = self->buttons;

		//LEFT
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			self->isLMBPressed = 1;

			self->lastLMBPos = mPos; //in camera mode drag this avoids giving an unwanted jump

			if (self->buttons->currentHoveredID != None)
			{
				self->buttons->currentPressedID = self->buttons->currentHoveredID;
			}
			if (self->buttons->currentPressedID == Drag)
			{
				self->isDraggingWindow = true;
				self->lastLMBPos = mPos;

			}
		}
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
		{
			self->isLMBPressed = 0;

			//Close and minimize actions are here because we need access to window
			// As buttons have access to functionality of telemetry, they should also be here
			if (self->buttons->currentPressedID == Close)
			{
				glfwSetWindowShouldClose(window, GLFW_TRUE);
			}
			else if (self->buttons->currentPressedID == Minimize)
			{
				glfwIconifyWindow(window);
			}
			self->isDraggingWindow = false;

			self->buttons->currentPressedID = None;
		}

		//RIGHT
		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
		{

		}


		//MIDDLE
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
		{
			self->isMMBPressed = 1;
			self->isLMBPressed = 0;

			self->lastMMBPos = mPos; //this avoids giving an unwanted jump

		}
		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
		{
			self->isMMBPressed = 0;
		}
	}




	static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) 
	{
		InputGLFW* self = static_cast<InputGLFW*>(glfwGetWindowUserPointer(window));
		Camera* camera = self->camera;

		float scrollTranslationSpeedFactor = 5;

		if (yoffset > 0)
		{
			camera->cameraPos = camera->cameraPos + camera->forward * self->translationSpeed * scrollTranslationSpeedFactor;

		}
		else if (yoffset < 0)
		{
			camera->cameraPos = camera->cameraPos - camera->forward * self->translationSpeed * scrollTranslationSpeedFactor;

		}

	}

	void getPos(GLFWwindow* window, p2& mPos) 
	{
		double xpos1, ypos1;
		glfwGetCursorPos(window, &xpos1, &ypos1);

		mPos = { (float)xpos1,(float)(windowHeight - ypos1) };
	}
};