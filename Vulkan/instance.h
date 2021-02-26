#pragma once

// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "main.h"

//std::vector<const char*> validationLayers;

namespace Engine {
	class mainProgram;
	class Instance {
		public:
			Instance(Engine::mainProgram** mainProgramPtr);
			~Instance();
			std::vector<const char*> getRequiredExtensions();
			bool checkValidationLayerSupport();
			VkInstance& getInstance();
		private:
			VkInstance instance;
	};
}