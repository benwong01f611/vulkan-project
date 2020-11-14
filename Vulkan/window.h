#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class mainProgram;
	class Window {
		public:
			Window(mainProgram** mainProgramPtr, GLFWwindow** windowPtr);
			~Window();

			int windowWidth;
			int windowHeight;

			static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
		private:
			GLFWwindow** window;
	};
};