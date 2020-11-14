#include "window.h"
#include "main.h"
#include "keyInput.h"

namespace Engine {
	Window::Window(mainProgram** mainProgramPtr) {
		mainProg = mainProgramPtr;
		windowHeight = 600;
		windowWidth = 800;
		glfwInit(); // Initializes GLFW library

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // GLFW is originally designed for OpenGL, NO_API for disabling OpenGL

		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Allow the window to resize

		window = glfwCreateWindow(windowWidth, windowHeight, "Vulkan", nullptr, nullptr); // GLFW window, defines window width, height, title, which monitor to show and the last parameter is for OpenGL, leave it nullptr

		// Set the framebuffersizecallback function
		// Store an arbitrary pointer
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);

		glfwSetKeyCallback(window, keyInput::keyCallback);

		(*mainProgramPtr)->framebufferResized = false;
	}
	Window::~Window() {
		glfwDestroyWindow(window); // Destroy the application window
	}
	void Window::framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<mainProgram*>(glfwGetWindowUserPointer(window));
		app->framebufferResized = true;
	}

	GLFWwindow** Window::getWindow()
	{
		return &window;
	}


}