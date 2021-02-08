#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "main.h"

namespace Engine {
	class mainProgram;
	class DescriptorSet {
	public:
		DescriptorSet(mainProgram** mainProgramPtr);
		VkDescriptorSetLayout* getDescriptorSetLayout();
		std::vector<VkDescriptorSet>* getDescriptorSets();
		void createDescriptorSets();
	private:
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;
		mainProgram** mainProg;
	};
}