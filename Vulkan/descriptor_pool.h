#pragma once
#include "main.h"
namespace Engine {
	class mainProgram;
	class DescriptorPool {
	public:
		DescriptorPool(mainProgram** mainProgramPtr);
		~DescriptorPool();
		VkDescriptorPool* getDescriptorPool();
	private:
		mainProgram** mainProg;
		VkDescriptorPool descriptorPool;
	};
}