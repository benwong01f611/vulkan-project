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
#include "commandPool.h"
#include "memory.h"
#include "image.h"
#include "framebuffer.h"
#include "buffer.h"
#include "commandBuffer.h"
#include "image.h"
#include "model.h"
#include "descriptor_pool.h"
#include "semaphores.h"
#include "keyInput.h"
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
	class CommandPool;
	class Memory;
	class Image;
	class FrameBuffer;
	class Buffer;
	class CommandBuffer;
	class DescriptorPool;
	class Semaphores;
	class KeyInput;
	class mainProgram {
	public:
		mainProgram();
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
		CommandPool* commandPool;
		Memory* memory;
		Image* image;
		FrameBuffer* frameBuffer;
		Buffer* buffer;
		CommandBuffer* commandBuffer;
		Model* model;
		bool framebufferResized;
		mainProgram** mainProgramPtr;
		DescriptorPool* descriptorPool;
		Semaphores* semaphores;
		KeyInput keyInput;
		void mainLoop();
		void cleanupSwapChain();
		void recreateSwapChain();
		void keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
	private:
		void drawFrame();
		size_t currentFrame = 0;
	};
	
	
}