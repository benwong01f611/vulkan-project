#pragma once
#include "main.h"
namespace Engine {
	class mainProgram;
	class Descriptor_Pool {
	public:
		Descriptor_Pool(mainProgram** mainProgramPtr);
	private:
		mainProgram** mainProg;
		VkDescriptorPool descriptorPool;
	};
}