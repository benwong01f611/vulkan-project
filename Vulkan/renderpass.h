#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"
namespace Engine {
	class RenderPass {
	public:
		RenderPass(mainProgram** mainProgramPtr);
		~RenderPass();
		mainProgram** getParent();
		VkFormat findDepthFormat();
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	private:
		mainProgram** mainProg;
		VkRenderPass renderPass;
	};
}