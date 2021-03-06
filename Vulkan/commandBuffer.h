#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class mainProgram;
	class Device;
	class SwapChain;
	class RenderPass;
	class CommandPool;
	class FrameBuffer;
	class Pipeline;
	class Model;
	class CommandBuffer {
	public:
		CommandBuffer(Device& deviceRef, SwapChain& swapChainRef, RenderPass& renderPassRef, CommandPool& commandPoolRef, Pipeline& pipelineRef, DescriptorSet& descriptorSetRef);
		~CommandBuffer();
		VkCommandBuffer beginSingleTimeCommands();
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void createCommandBuffers(FrameBuffer& framebufferRef, Model& model);
		std::vector<VkCommandBuffer>* getCommandBuffers();
		void destroyCommandBuffers();
	private:
		std::vector<VkCommandBuffer> commandBuffers; // Command buffers
		SwapChain& swapChain;
		RenderPass& renderPass;
		CommandPool& commandPool;
		Pipeline& pipeline;
		DescriptorSet& descriptorSet;
		Device& device;
	};
}