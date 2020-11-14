#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "main.h"
#include "globalVariables.h"

//bool enableValidationLayers;
//VkInstance instance;

namespace Engine {
	class mainProgram;
	class Debug :public globalVar{
	public:
		Debug(mainProgram** mainProgramPtr, VkInstance* instancePtr);
		~Debug();
		static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);

		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE; // Vulkan debug messenger
	private:
		VkInstance* instance;
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance vkinstance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);

	};
}