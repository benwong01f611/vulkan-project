#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "main.h"

namespace Engine {
	class Instance;
	class Surface;
	class Debug;
	class Device {
	public:
		void pickPhysicalDevice();
		Device(Instance& instanceRef, Surface& surfaceRef, Debug& debuggerRef);
		~Device();
		bool isDeviceSuitable(VkPhysicalDevice device);
		struct QueueFamilyIndices;
		VkSampleCountFlagBits getMaxUsableSampleCount();
		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
		bool checkDeviceExtensionSupport(VkPhysicalDevice device);
		VkSampleCountFlagBits& getMSAASamples(bool isMax);
		VkPhysicalDevice& getPhysicalDevice();
		VkDevice& getLogicalDevice();
		VkQueue& getGraphicsQueue();
		VkQueue& getPresentQueue();
		
	private:
		//VkPhysicalDevice* physicalDevice;
		VkPhysicalDevice physicalDevice;
		VkSampleCountFlagBits maxMSAASamples = VK_SAMPLE_COUNT_1_BIT;
		VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
		VkQueue graphicsQueue; // For handling the queue from logical device
		VkQueue presentQueue; // For handling the present queue
		VkDevice logicalDevice;
		Instance& instance;
		Surface& surface;
		Debug& debugger;
	};
}