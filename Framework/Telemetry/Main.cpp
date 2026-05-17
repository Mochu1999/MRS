


#include "Common.hpp"
#include "Graphics.hpp"

#include "SettingsTelemetry.hpp"

#include "Telemetry.hpp"
#include "TelemetryUI.hpp"
#include "TransmitterPC.hpp"



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

	AllPointers allPointers(&camera);

	glfwIput(window, &allPointers, keyboardEventCallback, mouseEventCallback, scrollCallback);
	
	

	while (!glfwWindowShouldClose(window))
	{
		getPos(window, mPos);
		if (isRunning)
		{
			clearScreen();


			telemetry.update();
			ui.draw();
			

			keyboardRealTimePolls(window, camera);
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




