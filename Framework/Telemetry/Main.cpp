

#include "Common.hpp"
#include "Graphics.hpp"



//#include "IncludesTelemetry.hpp"
#include "Time.hpp"
#include "KeyMouseImputs.hpp"
#include "FilesManagement.hpp"
#include "SettingsTelemetry.hpp"


#include "Lines3D.hpp"
#include "Lines2D.hpp"
#include "Lines2D_Instanced.hpp"
#include "Circles.hpp"
#include "Polygons.hpp"
#include "Polygons2D.hpp"
#include "Spheres.hpp"
#include "Polyhedra.hpp"
#include "Text.hpp"


#include "Globe.hpp"

#include "Delaunay2D.hpp"

#include "Plot.hpp"
#include "Autopilot.hpp"
#include "Axis.hpp"
#include "Lourdes.hpp"
#include "Overlay2D.hpp"
#include "ProgressBar.hpp"

#include "Telemetry.hpp"

int main(void)
{
	GLFWwindow* window = initialize();



	TimeStruct tm;

	Shader shader3D("resources/shaders/shader3D.shader");
	Shader shader2D("resources/shaders/shader2D.shader");
	Shader shader2D_Instanced("resources/shaders/shader2D_Instanced.shader");
	Shader shaderText("resources/shaders/shaderText.shader");
	Camera camera(window);
	cameraLocationsInitialization(shader3D, shader2D, shader2D_Instanced, shaderText, camera);



	Lourdes lourdes(shader3D, camera);

	Telemetry telemetry(shader3D, shader2D, shader2D_Instanced, shaderText, camera, tm, lourdes);




	Settings settings(camera);

	AllPointers allPointers(&camera, &lourdes);
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


			

			lourdes.draw();
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




