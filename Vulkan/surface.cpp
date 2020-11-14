#include "surface.h"
#include "window.h"
#include <iostream>

/*VkInstance instance;
GLFWwindow* window;
VkSurfaceKHR surface;*/

namespace Engine {
	Surface::Surface(mainProgram** mainProgramPtr, VkInstance* instancePtr, VkSurfaceKHR* surfacePtr, GLFWwindow** windowPtr) {
		instance = instancePtr;
		surface = surfacePtr;
		window = windowPtr;
		// Create window surface with glfwCreateWindowSurface
		if (glfwCreateWindowSurface(*instance, *window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}
	Surface::~Surface() {
		vkDestroySurfaceKHR(*instance, *surface, nullptr);
	}
}