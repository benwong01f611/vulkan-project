#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"
namespace Engine {
	class Device;
	class SwapChain;
	class RenderPass {
	public:
		RenderPass(Device& deviceRef, SwapChain& swapchainRef);
		~RenderPass();
		VkFormat findDepthFormat();
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkRenderPass& getRenderPass();
	private:
		VkRenderPass renderPass;
		Device& device;
		SwapChain& swapChain;
	};
}