#pragma once

// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "main.h"
namespace Engine {
	class SwapChain;
	class Device;
	class Semaphores {
		public:
			Semaphores(SwapChain& swapChainref, Device& deviceref);
			~Semaphores();
			std::vector<VkSemaphore>& getImageAvailableSemaphores();
			std::vector<VkSemaphore>& getRenderFinishedSemaphores();
			std::vector<VkFence>& getInFlightFences();
			std::vector<VkFence>& getImagesInFlight();
			const int MAX_FRAMES_IN_FLIGHT = 2;
		private:
			std::vector<VkSemaphore> imageAvailableSemaphores; // Store semaphores
			std::vector<VkSemaphore> renderFinishedSemaphores; // Store finished semaphores
			std::vector<VkFence> inFlightFences; // Fences are for synchronization, can be signaled and waited for
			std::vector<VkFence> imagesInFlight;
			SwapChain& swapChain;
			Device& device;
	};
}