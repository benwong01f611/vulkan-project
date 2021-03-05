#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"
namespace Engine {
	class Device;
	class CommandPool {
	public:
		CommandPool(Device& deviceRef);
		~CommandPool();
		VkCommandPool& getCommandPool();
	private:
		VkCommandPool commandPool;
		Device& device;
	};
}
