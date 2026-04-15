//HAY UN CONFLICTO DE FUNCIONALIDAD Y FORMATO ENTRE BinariesManager.hpp Y FilesManagement.hpp


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
	//main reason to add all the shaders there is to initialize the associated matrices in an encapsulated way
	// IT ISN'T REASONABLE TO HAVE THE SHADER INITIALIZATION IN CAMERA, ENCAPSULATE IT ELSEWHERE
	Camera camera(window, shader3D, shader2D, shader2D_Instanced, shaderText, gv);



	Text text("resources/Glyphs/Helvetica/Helvetica.otf", 36);
	text.addText({ {{ 10,950 }, tm.fps, " fps"},{{10,1000},tm.currentTime, " s"} });
	Text textAux("resources/Glyphs/Helvetica/Helvetica.otf", 48);


	Lines2D centerCross;
	centerCross.addSet({
		{gv.centerWindow.x - 20,gv.centerWindow.y},{gv.centerWindow.x + 20,gv.centerWindow.y},
		{gv.centerWindow.x,gv.centerWindow.y - 20},{gv.centerWindow.x,gv.centerWindow.y + 20} });
	centerCross.indices = { 0,1,2,3 };



	Axis axis(shader3D, gv);
	Light ship(shader3D, camera, gv);

	float pbValue = 0.5f;

	Overlay2D overlay(shader2D, camera);
	Graphic graphic(shader2D, shader2D_Instanced, shaderText, camera, tm, "A*cos(x)", { 1400,100 }, graf1Val);
	Graphic graphic2(shader2D, shader2D_Instanced, shaderText, camera, tm, "rudderAngle", { 1400,400 }, ship.rudderAngle);
	ProgressBar pb(shader2D, shader2D_Instanced, shaderText, camera, tm, p2{ 1350,700 },"Battery", pbValue);
	Autopilot autopilot(shader2D, shaderText, shader2D_Instanced, gv, tm);

	//Offshore offshore(shader3D, shader2D, shaderText, camera, gv, tm);

	Settings settings(camera, gv, autopilot);

	AllPointers allPointers(&camera, &gv, &autopilot, &ship);
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


			ship.draw();


			overlay.draw();

			graf1Val = cosPlot(c);
			graphic.draw();
			graphic2.draw();
			pb.draw();



			transparent();
			shader2D.bind();
			shader2D.setUniform("u_Model", identityMatrix);
			shader2D.setUniform("u_Color", 1, 1, 1, 0.5);
			glLineWidth(2);
			centerCross.draw();
			glLineWidth(1);
			opaque();

			keyboardRealTimePolls(window, gv, camera, autopilot);
			camera.updateCamera();



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


