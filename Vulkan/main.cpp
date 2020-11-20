#include "main.h"
#include <iostream>


Engine::mainProgram::mainProgram()
{
}


void Engine::mainProgram::init(mainProgram** mainProgPtr) {
	mainProgramPtr = mainProgPtr;

	memory = new Engine::Memory(mainProgramPtr);

	window = new Engine::Window(mainProgramPtr);
	debugger = new Engine::Debug(mainProgramPtr);
	instance = new Engine::Instance(mainProgramPtr);
	surface = new Engine::Surface(mainProgramPtr);
	device = new Engine::Device(mainProgramPtr);
	swapchain = new Engine::SwapChain(mainProgramPtr);
	renderPass = new Engine::RenderPass(mainProgramPtr);
	descriptorSet = new Engine::DescriptorSet(mainProgramPtr);
	pipeline = new Engine::Pipeline(mainProgramPtr);
	commandPool = new Engine::CommandPool(mainProgramPtr);
}

int main() {
//	Engine::mainProgram prog;
	Engine::mainProgram* prog;
	
	prog = new Engine::mainProgram();
	prog->init(&prog);
	return 0;
}
