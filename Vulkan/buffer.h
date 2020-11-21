#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"
namespace Engine {
	class mainProgram;
	class Buffer {
	public:
		Buffer(mainProgram** mainProgramPtr);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	private:
		mainProgram** mainProg;
	};
}