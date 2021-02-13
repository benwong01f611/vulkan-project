#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class mainProgram;
	class KeyInput;
	class Window {
		public:
			Window(mainProgram** mainProgramPtr,KeyInput& keyInputref);
			~Window();

			int windowWidth;
			int windowHeight;

			static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
			GLFWwindow** getWindow();
		private:
			GLFWwindow* window;
			mainProgram** mainProg;
			KeyInput& keyInput;
	};
};