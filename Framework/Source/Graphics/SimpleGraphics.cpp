#include "Graphics.hpp"




// to not render what is not visible to the camera:
//glEnable(GL_CULL_FACE);       // Enable face culling
//glCullFace(GL_BACK);          // Cull back faces
//glFrontFace(GL_CCW);          // Counter-clockwise winding is front-facing


//Default initialization without inputs
GLFWwindow* initialize()
{
	glfwInit();


	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwWindowHint(GLFW_SAMPLES, 4);  // Request 4x MSAA for antialiasing
	glEnable(GL_MULTISAMPLE);

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	//fullscreen
	//GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "CM's Framework", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "CM's Framework", NULL, NULL);

	/*GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);

	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "CM's Framework", nullptr, nullptr);
	glfwSetWindowMonitor(window, nullptr, 0, 0, mode->width, mode->height, mode->refreshRate);*/


	/* this will be useful to save width and height
	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "PR0JET R4CC00N", NULL, NULL);
	*/


	glfwMakeContextCurrent(window);
	//glewExperimental = GL_TRUE;
	glewInit();
	glfwSwapInterval(1); //0 is vsync disable

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);




	return window;
}

//specific version where you set the window's height and name
GLFWwindow* initialize(float windowWidth_, float windowHeight_, const char* windowName)
{
	windowWidth = windowWidth_;
	windowHeight = windowHeight_;
	glfwInit();
	glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//stays in front of the window
	glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

	glfwWindowHint(GLFW_SAMPLES, 4);  // Request 4x MSAA for antialiasing
	glEnable(GL_MULTISAMPLE);

	//glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

	//fullscreen
	//GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, "CM's Framework", glfwGetPrimaryMonitor(), NULL);
	GLFWwindow* window = glfwCreateWindow(windowWidth, windowHeight, windowName, NULL, NULL);



	glfwMakeContextCurrent(window);
	//glewExperimental = GL_TRUE;
	glewInit();
	glfwSwapInterval(1); //0 is vsync disable

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);


	glfwSetWindowPos(window, 1000, 200);

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

