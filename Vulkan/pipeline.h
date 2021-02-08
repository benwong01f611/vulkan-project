#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "main.h"

namespace Engine {
	class SwapChain;
	class Pipeline {
	public:
		Pipeline(mainProgram** mainProgramPtr);
		~Pipeline();
		VkPipeline* getGraphicsPipeline();
		VkPipelineLayout* getPipelineLayout();
		static std::vector<char> readFile(const std::string& filename);
		VkShaderModule createShaderModule(const std::vector<char>& code);
	private:
		VkPipelineLayout pipelineLayout; // Pipeline layout
		VkPipeline graphicsPipeline;
		VkDevice* logicalDevice;
	};
}