#include "surface.h"
#include "window.h"
#include <iostream>

/*VkInstance instance;
GLFWwindow* window;
VkSurfaceKHR surface;*/

namespace Engine {
	Surface::Surface(Instance& instanceRef, Window& windowRef) : instance(instanceRef.getInstance()), window(windowRef.getWindow()) {
		// Create window surface with glfwCreateWindowSurface
		if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface!");
		}
	}
	Surface::~Surface() {
		vkDestroySurfaceKHR(instance, surface, nullptr);
	}

	VkSurfaceKHR& Surface::getSurface()
	{
		return surface;
	}
	void Surface::initSurface()
	{
		
	}
}