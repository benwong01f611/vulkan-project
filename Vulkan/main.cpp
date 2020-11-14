#include "main.h"
#include <iostream>


Engine::mainProgram::mainProgram(mainProgram** mainProgPtr)
{
	window = new Engine::Window(mainProgramPtr);
	surface = new Engine::Surface(mainProgramPtr);
	instance = new Engine::Instance(mainProgramPtr);
	debugger = new Engine::Debug(mainProgramPtr);
	device = new Engine::Device(mainProgramPtr);
	surface->initSurface();
	swapchain = new Engine::SwapChain(mainProgramPtr);
	renderPass = new RenderPass(mainProgramPtr);
	descriptorSet = new DescriptorSet(mainProgramPtr);
	pipeline = new Pipeline(mainProgramPtr);
}

int main() {
//	Engine::mainProgram prog;
	Engine::mainProgram* prog;
	prog = new Engine::mainProgram(&prog);
	return 0;
}
