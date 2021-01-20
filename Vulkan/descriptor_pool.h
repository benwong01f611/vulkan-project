#pragma once
#include "main.h"
namespace Engine {
	class mainProgram;
	class DescriptorPool {
	public:
		DescriptorPool(mainProgram** mainProgramPtr);
		VkDescriptorPool* getDescriptorPool();
	private:
		mainProgram** mainProg;
		VkDescriptorPool descriptorPool;
	};
}