


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


	Polyhedra sphere;

	sphere.addSphere(1, { 0,0,0 },10000);

	Polygons2D circles;
	//vector<p2>points = createRandomPoints(150, { 100,100 }, { 600,600 });
	vector<p2>points = { {0.0848953,0},{-0.108229,0.0991462},{0.016536,-0.188419},{0.135919,0.177282},{-0.248971,-0.0440395},{-0.149615,-0.288073} };
	for (auto& point : points)
	{
		point *= 100;
		point += {300, 300};
	}
	std::vector<unsigned int> indices = delaunayTriangulation(points);

	circles.createCircle(7, points);

	Lines2D debugDelaunay;
	debugDelaunay.addDelaunaySet(points, indices);


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



			/*lourdesGraphic.draw();*/
			telemetry.update();

			transparent();
			shader3D.bind();
			shader3D.setUniform("u_Color", 1, 0, 0, 1);
			shader3D.setUniform("u_fragmentMode", 0);
			sphere.draw();
			shader3D.setUniform("u_Color", 0, 0, 1, 1);

			shader2D.bind();
			shader2D.setUniform("u_Color", 1, 0, 0, 1);
			glLineWidth(2);
			circles.draw();
			debugDelaunay.draw();

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




