#pragma once

// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "main.h"

//std::vector<const char*> validationLayers;

namespace Engine {
	class mainProgram;
	class Debug;
	class Instance {
		public:
			Instance(Debug& debuggerRef);
			~Instance();
			std::vector<const char*> getRequiredExtensions();
			bool checkValidationLayerSupport();
			VkInstance& getInstance();
		private:
			VkInstance instance;
			Debug& debugger;
	};
}