#pragma once




//Pointers can be re-seated while references cannot. But we are not re seating anything so whatever
struct AllPointers {
	Camera* camera;

	AllPointers(Camera* camera_)
		:camera(camera_) {
	}
};

//The standard is to use callbacks for one-time event (typing, increase something once per press) and another function
//  for per-frame polling that updates each frame or based on deltaTime for smooth changes in a variable over frames


void keyboardEventCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {

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

				//cameraModes //updateCamera in camera.cpp also need to be updated if ths is to change
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
void keyboardRealTimePolls(GLFWwindow* window, Camera& camera) {

	//The rest of the logic is in updateCamera

	// Rotation

	if (camera.cameraMode == camera.drag || camera.cameraMode == camera.FPS)
	{
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			camera.calculateForward(camera.forward, camera.rotationSpeed, camera.right);

		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			camera.calculateForward(camera.forward, -camera.rotationSpeed, camera.right);

		if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
			camera.calculateForward(camera.forward, -camera.rotationSpeed, camera.up);

		if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
			camera.calculateForward(camera.forward, camera.rotationSpeed, camera.up);


		//translation
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.cameraPos = camera.cameraPos + camera.forward * camera.translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.cameraPos = camera.cameraPos - camera.forward * camera.translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.cameraPos = camera.cameraPos - camera.right * camera.translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.cameraPos = camera.cameraPos + camera.right * camera.translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.cameraPos.y += camera.translationSpeed;

		if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
			camera.cameraPos.y -= camera.translationSpeed;
	}
	if (camera.cameraMode == camera.centered)
	{

	}



}



void getPos(GLFWwindow* window, p2& mPos) {
	double xpos1, ypos1;
	glfwGetCursorPos(window, &xpos1, &ypos1);

	mPos = { (float)xpos1,(float)(windowHeight - ypos1) };
}

void mouseEventCallback(GLFWwindow* window, int button, int action, int mods) {
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


float scrollTranslationSpeedFactor = 20;

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
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




//An alternative in structure to keyboardRealTimePolls with the same functionality
//functions are binded to each key with lambda functions and in pollBindings if the key is pressed it gets executed
/*
struct KeyBinding {
	int key;
	std::function<void(Camera&)> onPress;
};


struct KeyBindingsManager {

	Camera& camera;
	std::vector<KeyBinding> keyBindings;

	KeyBindingsManager(Camera& cam)
		: camera(cam)
	{
		keyBindings = {
			//Rotation
			{ GLFW_KEY_UP,    [&](Camera& c) { c.calculateForward(c.forward,  c.rotationSpeed,   c.right); } },
			{ GLFW_KEY_DOWN,  [&](Camera& c) { c.calculateForward(c.forward, -c.rotationSpeed,   c.right); } },
			{ GLFW_KEY_RIGHT, [&](Camera& c) { c.calculateForward(c.forward, -c.rotationSpeed,   c.up); } },
			{ GLFW_KEY_LEFT,  [&](Camera& c) { c.calculateForward(c.forward,  c.rotationSpeed,   c.up); } },
			// Translation:
			{ GLFW_KEY_W,     [&](Camera& c) { c.cameraPos += c.forward * c.translationSpeed; } },
			{ GLFW_KEY_S,     [&](Camera& c) { c.cameraPos -= c.forward * c.translationSpeed; } },
			{ GLFW_KEY_A,     [&](Camera& c) { c.cameraPos -= c.right * c.translationSpeed; } },
			{ GLFW_KEY_D,     [&](Camera& c) { c.cameraPos += c.right * c.translationSpeed; } },
			{ GLFW_KEY_Q,     [&](Camera& c) { c.cameraPos.y += c.translationSpeed; } },
			{ GLFW_KEY_Z,     [&](Camera& c) { c.cameraPos.y -= c.translationSpeed; } },
		};
	}


	void pollBindings(GLFWwindow* window) {
		for (auto& binding : keyBindings) {
			if (glfwGetKey(window, binding.key) == GLFW_PRESS) {
				binding.onPress(camera);
			}
		}
	}



};
*/
//initialized as
//	KeyBindingsManager inputMgr(camera);
//	inputMgr.pollBindings(window);