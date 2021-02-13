#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "main.h"

//bool enableValidationLayers;
//VkInstance instance;

namespace Engine {
	class mainProgram;
	class Debug{
	public:
		Debug(mainProgram** mainProgramPtr);
		~Debug();
		static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		// If it is not in debug stage, disable the validation layers, else enable it
		#ifdef NDEBUG
				const static bool enableValidationLayers = false;
		#else
				const static bool enableValidationLayers = true;
		#endif
		
		void initDebug();
		const std::vector<const char*> validationLayers = {
				"VK_LAYER_KHRONOS_validation"
		};;
		VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE; // Vulkan debug messenger
	private:
		VkInstance* instance;
		static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
			VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
			VkDebugUtilsMessageTypeFlagsEXT messageType,
			const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
			void* pUserData);
		VkResult CreateDebugUtilsMessengerEXT(VkInstance vkinstance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
		mainProgram** mainProgPtr;

	};
}