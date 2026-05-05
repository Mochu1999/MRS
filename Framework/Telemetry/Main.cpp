

#include "Common.hpp"
#include "Graphics.hpp"

#include "Time.hpp"
#include "Axis.hpp"
#include "SettingsTelemetry.hpp"
#include "KeyMouseImputs.hpp"

#include "IncludesTelemetry.hpp"
#include "LourdesGraphic.hpp"
#include "Telemetry.hpp"

int main(void)
{
	GLFWwindow* window = initialize();

	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2D_Instanced("resources/shaders/shader2D_Instanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	Camera camera(window);
	cameraLocationsInitialization(shader3D, shader2D, shader2D_Instanced, shaderText, camera);


	TimeStruct tm;

	LourdesGraphic lourdesGraphic(shader3D, camera);
	Telemetry telemetry(shader3D, shader2D, shader2D_Instanced, shaderText, camera, tm, lourdesGraphic);



	Settings settings(camera);

	AllPointers allPointers(&camera, &lourdesGraphic);
	glfwSetWindowUserPointer(window, &allPointers);
	glfwSetKeyCallback(window, keyboardEventCallback);
	glfwSetMouseButtonCallback(window, mouseEventCallback);
	glfwSetScrollCallback(window, scrollCallback);


	shaderText.bind();
	shaderText.setUniform("u_Color", 1.0f, 1.0f, 1.0f);

	while (!glfwWindowShouldClose(window))
	{
		getPos(window, mPos);
		if (isRunning)
		{
			tm.update();

			clearScreen();



			lourdesGraphic.draw();
			telemetry.update();



			keyboardRealTimePolls(window, camera);
			camera.updateCamera();
			//updating shader location
			shader3D.bind();
			shader3D.setUniform("u_View", camera.viewMatrix);

		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	settings.write();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}




