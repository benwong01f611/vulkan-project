#include "main.h"
#include <iostream>


Engine::mainProgram::mainProgram()
{
}

Engine::mainProgram::~mainProgram()
{
	delete image;
	delete frameBuffer;
	//delete commandBuffer;
	delete pipeline;
	delete renderPass;
	delete model;
	delete swapchain;
	delete descriptorPool;
	delete descriptorSet;
	delete commandPool;
	delete device;
	delete debugger;
	delete surface;
	delete instance;
	delete window;

}


void Engine::mainProgram::init(mainProgram** mainProgPtr) {
	mainProgramPtr = mainProgPtr;

	memory = new Engine::Memory(mainProgramPtr);
	//buffer = new Engine::Buffer(mainProgramPtr);
	commandBuffer = new Engine::CommandBuffer(mainProgramPtr);
	model = new Engine::Model(mainProgramPtr);

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
	image = new Engine::Image(mainProgramPtr);
	frameBuffer = new Engine::FrameBuffer(mainProgramPtr);
	image->createTexture();
	model->loadModel();
	model->createVertexBuffer();
	model->createIndexBuffer();
	model->createUniformBuffers();
	descriptorPool = new Engine::DescriptorPool(mainProgramPtr);
	descriptorSet->createDescriptorSets();
	
	// mainLoop drawing event
	//mainLoop();
	std::cout << "Program end!" << std::endl;
}

int main() {
//	Engine::mainProgram prog;
	Engine::mainProgram* prog;
	
	prog = new Engine::mainProgram();
	prog->init(&prog);
	return 0;
}
