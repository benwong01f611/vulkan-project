#include "main.h"
#include <iostream>


Engine::mainProgram::mainProgram()
{
}


void Engine::mainProgram::init(mainProgram** mainProgPtr) {
	mainProgramPtr = mainProgPtr;
	window = new Engine::Window(mainProgramPtr);
	debugger = new Engine::Debug(mainProgramPtr);
	instance = new Engine::Instance(mainProgramPtr);
	surface = new Engine::Surface(mainProgramPtr);
	device = new Engine::Device(mainProgramPtr);
	swapchain = new Engine::SwapChain(mainProgramPtr);
	renderPass = new RenderPass(mainProgramPtr);
	descriptorSet = new DescriptorSet(mainProgramPtr);
	pipeline = new Pipeline(mainProgramPtr);
}

int main() {
//	Engine::mainProgram prog;
	Engine::mainProgram* prog;
	
	prog = new Engine::mainProgram();
	prog->init(&prog);
	return 0;
}
