#pragma once

// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "window.h"
#include "instance.h"
#include "debug.h"
#include "surface.h"
#include "swapchain.h"
#include "device.h"
#include "renderpass.h"
#include "descriptor_set.h"
#include "pipeline.h"

namespace Engine {
	class Window;
	class Instance;
	class Debug;
	class Surface;
	class Device;
	class SwapChain;
	class RenderPass;
	class DescriptorSet;
	class Pipeline; 
	class Model;
	class mainProgram {
	public:
		mainProgram(mainProgram** mainProgPtr);
		~mainProgram();
		Window* window;
		Instance* instance;
		Debug* debugger;
		Surface* surface;
		Device* device;
		SwapChain* swapchain;
		RenderPass* renderPass;
		DescriptorSet* descriptorSet;
		Pipeline* pipeline;
		/*VkInstance vkInstance;
		VkPhysicalDevice physicalDevice;
		VkDevice logicalDevice;
		VkSurfaceKHR vkSurface;*/
		bool framebufferResized = false;
		mainProgram** mainProgramPtr;
	};
	
	
}