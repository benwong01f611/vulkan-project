#include "main.h"
#include <iostream>


Engine::mainProgram::mainProgram(mainProgram** mainProgPtr)
{
	window = new Engine::Window(mainProgramPtr, &glfwWindow);
	instance = new Engine::Instance(mainProgramPtr, &vkInstance);
	debugger = new Engine::Debug(mainProgramPtr, &vkInstance);
	device = new Engine::Device(mainProgramPtr, &vkInstance, &physicalDevice, &vkSurface);
	surface = new Engine::Surface(mainProgramPtr, &vkInstance, &vkSurface, &glfwWindow);
	swapchain = new Engine::SwapChain(mainProgramPtr, &vkSurface, &physicalDevice, &logicalDevice, &glfwWindow);
}

int main() {
//	Engine::mainProgram prog;
	Engine::mainProgram* prog;
	prog = new Engine::mainProgram(&prog);
	return 0;
}
