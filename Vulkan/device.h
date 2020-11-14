#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "main.h"
#include "globalVariables.h"
/*VkPhysicalDevice physicalDevice;
VkSampleCountFlagBits maxMSAASamples;
VkSampleCountFlagBits msaaSamples;
//VkInstance instance;
//std::vector<const char*> validationLayers;
VkDevice logicalDevice;
VkQueue graphicsQueue;
VkQueue presentQueue;
VkSurfaceKHR surface;
//bool enableValidationLayers;*/

namespace Engine {
	class mainProgram;
	class Device :public globalVar {
	public:
		void pickPhysicalDevice();
		Device(mainProgram** mainProgramPtr, VkInstance* instancePtr, VkPhysicalDevice* physicalDevicePtr, VkSurfaceKHR* surfacePtr);
		~Device();
		bool isDeviceSuitable(VkPhysicalDevice device);
		struct QueueFamilyIndices;
		VkSampleCountFlagBits getMaxUsableSampleCount();
		static QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, VkSurfaceKHR* surface);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		VkSampleCountFlagBits* getMSAASamples(bool isMax);
		VkPhysicalDevice* getPhysicalDevice();
		VkDevice* getLogicalDevice();
	private:
		VkPhysicalDevice* physicalDevice;
		VkInstance* instance;
		VkSampleCountFlagBits maxMSAASamples = VK_SAMPLE_COUNT_1_BIT;
		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		VkQueue graphicsQueue; // For handling the queue from logical device
		VkQueue presentQueue; // For handling the present queue
		VkDevice logicalDevice;
		VkSurfaceKHR* surface;
	};
}