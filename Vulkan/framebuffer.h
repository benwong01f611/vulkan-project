#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"
namespace Engine {
	class mainProgram;
	class FrameBuffer {
	public:
		FrameBuffer(mainProgram** mainProgramPtr);
		~FrameBuffer();
		std::vector<VkFramebuffer>& getSwapChainFramebuffers();
	private:
		mainProgram** mainProg;
		std::vector<VkFramebuffer> swapChainFramebuffers; // Vector to hold framebuffers
	};
}