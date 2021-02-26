#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {

	class mainProgram;
	class Surface {
	public:
		Surface(mainProgram** mainProgramPtr);
		~Surface();
		VkSurfaceKHR& getSurface();
		void initSurface();
	private:
		VkInstance& instance;
		VkSurfaceKHR surface;
	};
}