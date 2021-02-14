#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class mainProgram;
	class CommandBuffer {
	public:
		CommandBuffer(mainProgram** mainProgramPtr);
		~CommandBuffer();
		VkCommandBuffer beginSingleTimeCommands();
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void endSingleTimeCommands(VkCommandBuffer commandBuffer);
		void createCommandBuffers();
		std::vector<VkCommandBuffer>* getCommandBuffers();
		void destroyCommandBuffers();
	private:
		mainProgram** mainProg;
		std::vector<VkCommandBuffer> commandBuffers; // Command buffers
	};
}