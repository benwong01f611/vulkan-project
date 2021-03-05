#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class Instance;
	class Window;
	class Surface {
	public:
		Surface(Instance& instanceRef, Window& windowRef);
		~Surface();
		VkSurfaceKHR& getSurface();
		void initSurface();
	private:
		GLFWwindow*& window;
		VkInstance& instance;
		VkSurfaceKHR surface;
	};
}