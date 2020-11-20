#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"
namespace Engine {
	class mainProgram;
	class Memory {
	public:
		Memory(mainProgram** mainProgramPtr);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	private:
		mainProgram** mainProg;
	};
}