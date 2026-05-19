
//To do
//camera input, tesselation, text, shaders, botones

#include "Common.hpp"
#include "Graphics.hpp"

#include "SettingsTelemetry.hpp"

#include "TelemetryUI.hpp"

#include "TransmitterPC.hpp"

#include "KeyMouseImputs.hpp"

int main(void)
{
	GLFWwindow* window = initialize();

	//Unify in a single shader once only references of it exist only in UI
	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2DInstanced("resources/shaders/shader2DInstanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	Camera camera(window);
	initializeCameraLocations(shader3D, shader2D, shader2DInstanced, shaderText, camera);

	
	Telemetry telemetry;
	TelemetryUI ui(telemetry, shader3D, shader2D, shader2DInstanced, shaderText, camera);


	Settings settings(camera);
	InputGLFW inputGLFW(window, &camera,&telemetry);

	//AllPointers allPointers(&camera/*, &telemetry*/);
	//glfwIput(window, &allPointers, keyboardEventCallback, mouseEventCallback, scrollCallback);
	
	while (!glfwWindowShouldClose(window))
	{
		inputGLFW.getPos(window, mPos);
		if (isRunning)
		{
			clearScreen();

			//program's logic
			telemetry.update();
			//program's rendering
			ui.draw();
			

			inputGLFW.keyboardRealTimePolls(window);
			camera.updateCamera();

			updateCameraLocations(shader3D, camera);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	settings.write();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}




