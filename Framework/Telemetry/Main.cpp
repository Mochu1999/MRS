
// Force Model implementation. Addition of hull resistances. Linear interpotalor function.
// 


//To do
// Buttons en UI
// Bugs lineas plot, visualización de tiempos altos
//fijar dataroute en algún sitio definitivo(y círculos, icono, líneas. Cambiar nombre para que lo refleje), mediterranean map corner fix, safe zones fix
//visual speed water proportional to the ship's speed 
// Anemómetro, flechas 3D
//Change rendering values menu, change telemetry values with the mouse
//Power consumed


#include "Common.hpp"
#include "Graphics.hpp"

#include "SettingsTelemetry.hpp"

#include "Parameters.hpp"
#include "TelemetryUI.hpp"

#include "LoRa.hpp"

#include "InputGLFW.hpp"

#include "ForceModel.hpp"


int main()
{
	GLFWwindow* window = initialize(windowWidth, windowHeight, "MRS");

	//Unify in a single shader once only references of it exist only in UI
	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2DInstanced("resources/shaders/shader2DInstanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	Shader shaderText3D("resources/shaders/shaderText3D.shader");
	Shader shaderWater("resources/shaders/shaderWater.shader");
	Camera camera;
	initializeCameraLocations(shader3D, shader2D, shader2DInstanced, shaderText, shaderText3D, shaderWater, camera);


	Parameters p;
	LoRa lora(p);
	Buttons buttons(p.rudderAngle, p.sailAngle);
	TelemetryUI ui(p, shader3D, shader2D, shader2DInstanced, shaderText, shaderText3D, shaderWater, camera, buttons, lora);

	Settings settings(camera);
	InputGLFW inputGLFW(window, &camera, &p, &ui, &buttons);

	return 0;
	while (!glfwWindowShouldClose(window))
	{
		inputGLFW.getPos(window, mPos);
		if (isRunning) //Debugging purposes
		{
			clearScreen();
			buttons.update();

			//program's logic
			p.update();
			//program's rendering
			ui.draw();
			//sends message if value is updated
			lora.update();

			inputGLFW.customPolls();
			camera.updateCamera();

			updateCameraLocations(shader3D,shaderText3D, shaderWater, camera);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	settings.write();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}




