#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {

	class mainProgram;
	class Surface {
	public:
		Surface(mainProgram** mainProgramPtr, VkInstance* instance, VkSurfaceKHR* surface, GLFWwindow** window);
		~Surface();
	private:
		VkInstance* instance;
		VkSurfaceKHR* surface;
		GLFWwindow** window;
	};
}