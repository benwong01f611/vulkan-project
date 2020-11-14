#pragma once

// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "globalVariables.h"
#include "main.h"

//std::vector<const char*> validationLayers;

namespace Engine {
	class mainProgram;
	class Instance : public globalVar {
		public:
			Instance(Engine::mainProgram** mainProgramPtr, VkInstance* instancePtr);
			~Instance();
			std::vector<const char*> getRequiredExtensions();
			bool checkValidationLayerSupport();
		private:
			VkInstance* instance;
	};
}