#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class KeyInput;
	class Window {
		public:
			Window(KeyInput& keyInputref, bool& fbResized);
			~Window();

			int windowWidth;
			int windowHeight;

			static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
			GLFWwindow* & getWindow();
		private:
			GLFWwindow* window;
			KeyInput& keyInput;
			bool& framebufferResized;
	};
};