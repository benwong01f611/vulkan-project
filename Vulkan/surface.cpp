#include "surface.h"
#include "window.h"
#include <iostream>

/*VkInstance instance;
GLFWwindow* window;
VkSurfaceKHR surface;*/

namespace Engine {
	Surface::Surface(mainProgram** mainProgramPtr) {
		mainProg = mainProgramPtr;
		instance = (*mainProg)->instance->getInstance();
		// Create window surface with glfwCreateWindowSurface
		if (glfwCreateWindowSurface(*(*mainProg)->instance->getInstance(), *(*mainProg)->window->getWindow(), nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}
	Surface::~Surface() {
		vkDestroySurfaceKHR(*instance, surface, nullptr);
	}

	VkSurfaceKHR* Surface::getSurface()
	{
		return &surface;
	}
	void Surface::initSurface()
	{
		
	}
}