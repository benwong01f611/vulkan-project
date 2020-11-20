#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"
namespace Engine {
	class mainProgram;
	class CommandPool {
	public:
		CommandPool(mainProgram** mainProgramPtr);
		~CommandPool();
		mainProgram** mainProg;
		VkCommandPool* getCommandPool();
	private:
		VkCommandPool commandPool;
	};
}
