#include "Graphics.hpp"




// to not render what is not visible to the camera:
//glEnable(GL_CULL_FACE);       // Enable face culling
//glCullFace(GL_BACK);          // Cull back faces
//glFrontFace(GL_CCW);          // Counter-clockwise winding is front-facing

void initializeGlfw()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwWindowHint(GLFW_SAMPLES, 4);  // Request 4x MSAA for antialiasing

	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

}

void initializeGlew()
{
	//glewExperimental = GL_TRUE;
	glewInit();
	glfwSwapInterval(1); //0 to disable vsync

	//Enables multisample anti-aliasing (MSAA), must be asked at glfw
	glEnable(GL_MULTISAMPLE);

	//antialiasing configuration
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);



	////This line solve a bug where glyph text are not correctly rendered.
	//Means it will render from {0,0} to {w,h}, that is the full window
	//glViewport(0, 0, windowWidth, windowHeight);
}

//specific version where you set the window's height and name
GLFWwindow* initialize(float windowWidth_, float windowHeight_, const char* windowName)
{
	windowWidth = windowWidth_;
	//-1 to access Windows' bottom bar
	if (windowHeight_ == windowHeight)
		windowHeight--;
	else
		windowHeight = windowHeight_;

	initializeGlfw();


	//fullscreen
	//windowHeight-1 to access Window's bottom bar
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);

	glfwMakeContextCurrent(window);


	initializeGlew();

	//glfwSetWindowPos(window, 0, 0);

	return window;
}

//Default initialization without inputs
GLFWwindow* initialize()
{
	initializeGlfw();

	//fullscreen
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "MRS", NULL, NULL);


	glfwMakeContextCurrent(window);

	initializeGlew();

	glfwSetWindowPos(window, 0, 0);

	return window;
}





void clearScreen()
{
	opaque();
	glClearColor(40 / 255.0f, 40 / 255.0f, 40 / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}



void getCursorPos(GLFWwindow* window) {

	double intermX, intermY;

	glfwGetCursorPos(window, &intermX, &intermY);

	mPos = { static_cast<float>(intermX), static_cast<float>(windowHeight - intermY) };

}
void opaque() {
	glDepthMask(GL_TRUE);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
}


void transparent() {
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//deactivates the depth buffer, if activated the objects behind it won't be rendered
	//, irrelevant if there aren't multiple layers of transparent
	glDepthMask(GL_FALSE);
}

