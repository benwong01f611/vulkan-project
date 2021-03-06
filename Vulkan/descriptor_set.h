#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class Device;
	class SwapChain;
	class Image;
	class Model;
	class DescriptorPool;
	class DescriptorSet {
	public:
		DescriptorSet(Device& deviceRef, SwapChain& swapChainRef);
		VkDescriptorSetLayout& getDescriptorSetLayout();
		std::vector<VkDescriptorSet>& getDescriptorSets();
		void createDescriptorSets(DescriptorPool& descriptorPoolRef, Model& model, Image& image);
	private:
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;
		SwapChain& swapChain;
		Device& device;
	};
}