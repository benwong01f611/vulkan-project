#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

namespace Engine {
	class globalVar {
	protected:
		// If it is not in debug stage, disable the validation layers, else enable it
		#ifdef NDEBUG
				const bool enableValidationLayers = false;
		#else
				const bool enableValidationLayers = true;
		#endif

		const std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
		};;
	};

	
	/*VkInstance instance;
	VkSurfaceKHR surface;

	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkSampleCountFlagBits maxMSAASamples = VK_SAMPLE_COUNT_1_BIT;
	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;

	VkDevice logicalDevice;
	VkQueue graphicsQueue; // For handling the queue from logical device
	VkQueue presentQueue; // For handling the present queue


	VkSwapchainKHR swapChain; // Swap chain
	std::vector<VkImage> swapChainImages; // Images in swap chain
	VkFormat swapChainImageFormat; // 
	VkExtent2D swapChainExtent; //
	GLFWwindow* window;*/
}