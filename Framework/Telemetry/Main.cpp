
// Text refactorization. PFS counter
//To do
// Anemómetro, flechas 3D,
// camera new modes, sweepTriangulation, text, shaderText, shader3D, botones
//, fps counter
// Change to simulation view
// Simulation parts integration
//Change rendering values menu, change telemetry values with the mouse
//import values in csv
//Settings: CameraSensivity value, water
//solar panels

#include "Common.hpp"
#include "Graphics.hpp"

#include "SettingsTelemetry.hpp"

#include "TelemetryUI.hpp"

#include "TransmitterPC.hpp"

#include "InputGLFW.hpp"

int main(void)
{
	GLFWwindow* window = initialize();

	//Unify in a single shader once only references of it exist only in UI
	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2DInstanced("resources/shaders/shader2DInstanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	Camera camera;
	initializeCameraLocations(shader3D, shader2D, shader2DInstanced, shaderText, camera);

	
	Telemetry telemetry;
	TelemetryUI ui(telemetry, shader3D, shader2D, shader2DInstanced, shaderText, camera);
	TransmitterPC transmitter(telemetry);

	Settings settings(camera);
	InputGLFW inputGLFW(window, &camera,&telemetry);

	while (!glfwWindowShouldClose(window))
	{
		inputGLFW.getPos(window, mPos);
		if (isRunning) //TO BE USED ONLY FOR DEBUGGING PURPOSES
		{
			clearScreen();

			//program's logic
			telemetry.update();
			//program's rendering
			ui.draw();
			

			inputGLFW.customPolls(window);
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




