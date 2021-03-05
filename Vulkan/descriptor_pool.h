#pragma once
#include "main.h"
namespace Engine {
	class Device;
	class SwapChain;
	class DescriptorPool {
	public:
		DescriptorPool(Device& deviceRef, SwapChain& swapChainRef);
		~DescriptorPool();
		VkDescriptorPool& getDescriptorPool();
	private:
		VkDescriptorPool descriptorPool;
		Device& device;
		SwapChain& swapChain;
	};
}