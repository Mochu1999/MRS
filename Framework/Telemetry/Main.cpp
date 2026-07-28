
//Caso límite muchos mensajes acumulados en el serial, solo procesar de ";" a ";"
//Redactar funciones/requisitos de la antena de corto alcance con lo que hay, pasar por todo lo que se ha hecho y resumir en la propuesta de control

// 
// Documentation moved to the MRS google drive
// Text refactorization. PFS counter
//To do
// Anemómetro, flechas 3D, onboard cmakelists
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

#include "LoRa.hpp"

#include "InputGLFW.hpp"



int main(void)
{

	//GLFWwindow* window = initialize(windowWidth, windowHeight, "MRS");
	GLFWwindow* window = initialize(/*windowWidth,windowHeight,"MRS"*/);

	//Unify in a single shader once only references of it exist only in UI
	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2DInstanced("resources/shaders/shader2DInstanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	Camera camera;
	initializeCameraLocations(shader3D, shader2D, shader2DInstanced, shaderText, camera);

	Buttons buttons;
	
	Telemetry telemetry;
	TelemetryUI ui(telemetry, shader3D, shader2D, shader2DInstanced, shaderText, camera, buttons);
	LoRa lora(telemetry);

	Settings settings(camera);
	InputGLFW inputGLFW(window, &camera,&telemetry, &buttons);

	while (!glfwWindowShouldClose(window))
	{
		inputGLFW.getPos(window, mPos);
		if (isRunning) //Debugging purposes
		{
			clearScreen();
			buttons.update();

			//program's logic
			telemetry.update();
			//program's rendering
			ui.draw();
			//sends message if value is updated
			lora.update();

			inputGLFW.customPolls();
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




