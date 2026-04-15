#include "IncludesTelemetry.hpp"



int main(void)
{
	GlobalVariables gv;

	GLFWwindow* window = initialize();



	TimeStruct tm;

	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2D_Instanced("resources/shaders/shader2D_Instanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	//remove shaders from camera
	Camera camera(window, gv);
	cameraLocationsInitialization(shader3D, shader2D, shader2D_Instanced, shaderText, camera);


	//Autopilot won't be needed for the first iteration but it will be needed in next iterations
	Autopilot autopilot(shader2D, shaderText, shader2D_Instanced, gv, tm);

	Lourdes lourdes(shader3D, camera, gv);

	Telemetry telemetry(shader3D, shader2D, shader2D_Instanced, shaderText, camera, gv, tm, lourdes);




	Settings settings(camera, gv, autopilot);

	AllPointers allPointers(&camera, &gv, &autopilot, &lourdes);
	glfwSetWindowUserPointer(window, &allPointers);
	glfwSetKeyCallback(window, keyboardEventCallback);
	glfwSetMouseButtonCallback(window, mouseEventCallback);
	glfwSetScrollCallback(window, scrollCallback);


	int counter = 0;
	//return 0;
	//system("cls");
	camera.forward = { 1,0,0 };
	while (!glfwWindowShouldClose(window))
	{
		getPos(window, gv.mPos);
		if (gv.isRunning)
		{
			tm.update();

			clearScreen(gv);


			transparent();
			shaderText.bind();
			shaderText.setUniform("u_Color", 1.0f, 1.0f, 1.0f);

			lourdes.draw();




			telemetry.update();



			keyboardRealTimePolls(window, gv, camera, autopilot);
			camera.updateCamera();
			//updating shader location
			shader3D.bind();
			shader3D.setUniform("u_View", camera.viewMatrix);


			//break;

		}
		//isRunning = false;


		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	settings.write();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}




