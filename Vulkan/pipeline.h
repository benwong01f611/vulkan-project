#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include "main.h"

namespace Engine {
	class SwapChain;
	class Device;
	class SwapChain;
	class DescriptorSet;
	class RenderPass;
	class Pipeline {
	public:
		Pipeline(Device& deviceRef, SwapChain& swapChainRef, DescriptorSet& descriptorSetRef, RenderPass& renderPassRef);
		~Pipeline();
		VkPipeline& getGraphicsPipeline();
		VkPipelineLayout& getPipelineLayout();
		static std::vector<char> readFile(const std::string& filename);
		VkShaderModule createShaderModule(const std::vector<char>& code);
		void initPipeline();
	private:
		VkPipelineLayout pipelineLayout; // Pipeline layout
		VkPipeline graphicsPipeline;
		RenderPass& renderPass;
		Device& device;
		SwapChain& swapChain;
		DescriptorSet& descriptorSet;
	};
}