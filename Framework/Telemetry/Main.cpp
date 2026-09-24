
// OnBoard arduino implementation. CMake configuration for release. Rudder and Sail control with text fields and drag bars. Small changes to the UI. Working Release folder for whatever pc with VC_redist.x64. Deleted OpenCascade references
// 


//To do
// Buttons en UI
// Bugs lineas plot, visualización de tiempos altos
//fijar dataroute en algún sitio definitivo(y círculos, icono, líneas. Cambiar nombre para que lo refleje), mediterranean map corner fix, safe zones fix
//visual speed water proportional to the ship's speed 
// Anemómetro, flechas 3D
//Change rendering values menu, change telemetry values with the mouse
//Power consumed


//Caso límite muchos mensajes acumulados en el serial, solo procesar de ";" a ";"
//Icono de que hay o no hay antena

#include "Common.hpp"
#include "Graphics.hpp"

#include "SettingsTelemetry.hpp"

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


	Telemetry t;
	LoRa lora(t);
	Buttons buttons(t.rudderAngle, t.sailAngle);
	TelemetryUI ui(t, shader3D, shader2D, shader2DInstanced, shaderText, shaderText3D, shaderWater, camera, buttons, lora);

	Settings settings(camera);
	InputGLFW inputGLFW(window, &camera, &t, &ui, &buttons);

	//return 0;
	while (!glfwWindowShouldClose(window))
	{
		inputGLFW.getPos(window, mPos);
		if (isRunning) //Debugging purposes
		{
			clearScreen();
			buttons.update();

			//program's logic
			t.update();
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




