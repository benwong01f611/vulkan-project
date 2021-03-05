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
		DescriptorSet(Device& deviceRef, SwapChain& swapChainRef, Image& imageRef, Model& modelRef, DescriptorPool& descriptorPoolRef);
		VkDescriptorSetLayout& getDescriptorSetLayout();
		std::vector<VkDescriptorSet>& getDescriptorSets();
		void createDescriptorSets();
	private:
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;
		SwapChain& swapChain;
		Device& device;
		Image& image;
		Model& model;
		DescriptorPool& descriptorPool;
	};
}