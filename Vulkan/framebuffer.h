#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"
namespace Engine {
	class Device;
	class SwapChain;
	class Image;
	class RenderPass;
	class FrameBuffer {
	public:
		FrameBuffer(Device& deviceRef, SwapChain& swapChainRef, Image& imageRef, RenderPass& renderPassRef);
		~FrameBuffer();
		void initFrameBuffer();
		std::vector<VkFramebuffer>& getSwapChainFramebuffers();
	private:
		std::vector<VkFramebuffer> swapChainFramebuffers; // Vector to hold framebuffers
		SwapChain& swapChain;
		Image& image;
		RenderPass& renderPass;
		Device& device;
	};
}