

#include "Common.hpp"
#include "Graphics.hpp"

#include "Time.hpp"
#include "Axis.hpp"
#include "SettingsTelemetry.hpp"
#include "KeyMouseImputs.hpp"

#include "Globe.hpp"
#include "Plot.hpp"
#include "Overlay2D.hpp"
#include "ProgressBar.hpp"
#include "LourdesGraphic.hpp"
#include "Telemetry.hpp"

int main(void)
{
	GLFWwindow* window = initialize();

	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2DInstanced("resources/shaders/shader2DInstanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	Camera camera(window);
	initializeCameraLocations(shader3D, shader2D, shader2DInstanced, shaderText, camera);

	
	TimeStruct tm;

	LourdesGraphic lourdesGraphic(shader3D, camera);
	Telemetry telemetry(shader3D, shader2D, shader2DInstanced, shaderText, camera, tm, lourdesGraphic);
	
	Lines2DInstanced lines;
	lines.addInitialSet({ {0,0},{300,0}, {300,100}, {0,100}, {0,0} });
	lines.addInstances({ { { 1,1 },0, {100,100} } });
	lines.addMoreInstances({ {{1,1},radians(30),{100,100} }, { { 2,0.2 },radians(-90),{500,800}} });



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

			shader2DInstanced.bind();
			shader2DInstanced.setUniform("u_Color", 1, 1, 1, 1);
			lines.draw();

			lourdesGraphic.draw();
			telemetry.update();


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




