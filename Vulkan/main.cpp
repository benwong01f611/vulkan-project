#include "main.h"
#include <iostream>


Engine::mainProgram::mainProgram()
{
}

Engine::mainProgram::~mainProgram()
{
    cleanupSwapChain();
	delete descriptorSet;
	delete semaphores;
	delete commandPool;
	delete device;
	delete debugger;
	delete surface;
	delete instance;
	delete window;

}

Engine::KeyInput::keyboardKeys keys;

void Engine::mainProgram::cleanupSwapChain() {
    image->cleanImages();
    delete frameBuffer;
    commandBuffer->destroyCommandBuffers();
    delete pipeline;
    delete renderPass;
    model->destroyUniformBuffer();
    delete swapchain;
    delete descriptorPool;
}

void Engine::mainProgram::init(mainProgram** mainProgPtr) {
    mainProgramPtr = mainProgPtr;
    keyInput = KeyInput();
    memory = new Engine::Memory(mainProgramPtr);
    //buffer = new Engine::Buffer(mainProgramPtr);
    commandBuffer = new Engine::CommandBuffer(mainProgramPtr);
    model = new Engine::Model(mainProgramPtr,keys);

    window = new Engine::Window(mainProgramPtr,keyInput);
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
    commandBuffer->createCommandBuffers();
    semaphores = new Engine::Semaphores(*swapchain, *device);
}

void Engine::mainProgram::mainLoop() {
	// Keep the application running until error occurs or window is closed
	while (!glfwWindowShouldClose(*window->getWindow())) {
		glfwPollEvents(); // Poll any events while the window is not closed
		drawFrame(); // Keep drawing frames while the program is running
	}
	// When the window is closed, glfwWindowShouldClose(window) will turn true and breaks the loop
	// But the drawing and presentation operations may still going on
	// vkDeviceWaitIdle will wait for the operations to finish first before destroying the window
	vkDeviceWaitIdle(*device->getLogicalDevice());
}

void Engine::mainProgram::drawFrame() {
    // Wait for the frame to be finished
    // VK_TRUE means waiting for all fences
    // UINT64_MAX for disabling timeout
    std::vector<VkFence>& inFlightFences = semaphores->getInFlightFences();
    vkWaitForFences(*device->getLogicalDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

    uint32_t imageIndex;

    // Acquire next image index from swapchain to imageIndex, refers to the VkImage in swapChainImages, disables timeout with UINT64_MAX
    std::vector<VkSemaphore>& imageAvailableSemaphores = semaphores->getImageAvailableSemaphores();
    VkResult result = vkAcquireNextImageKHR(*device->getLogicalDevice(), *swapchain->getSwapChain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    // If the returned result is out of date (which means the swap chain is incompatible with surface and no longer be used for rendering, recreate swap chain and end this function
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    }
    // Swap chain can be present, but properties are no longer matched exactly
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    std::vector<VkFence>& imagesInFlight = semaphores->getImagesInFlight();
    // Wait if it has images in flight (using this image)
    if (imagesInFlight[imageIndex] != VK_NULL_HANDLE) {
        vkWaitForFences(*device->getLogicalDevice(), 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
    }
    // Mark the image as now being in use by this frame
    imagesInFlight[imageIndex] = inFlightFences[currentFrame];

    model->updateUniformBuffer(imageIndex);

    // Submit the command buffer
    VkSubmitInfo submitInfo = {};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
    VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
    submitInfo.waitSemaphoreCount = 1;
    // Which semaphore to wait on before execution begins
    submitInfo.pWaitSemaphores = waitSemaphores;
    // Which stages of the pipeline to wait
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    // Which command buffers to submit
    std::vector<VkCommandBuffer>& commandBuffers = *commandBuffer->getCommandBuffers();
    submitInfo.pCommandBuffers = &commandBuffers[imageIndex];
    std::vector<VkSemaphore>& renderFinishedSemaphores = semaphores->getRenderFinishedSemaphores();
    VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
    submitInfo.signalSemaphoreCount = 1;
    // Which semaphores to signal after execution
    submitInfo.pSignalSemaphores = signalSemaphores;

    // We need to manually reset the fence to the unsignaled state
    vkResetFences(*device->getLogicalDevice(), 1, &inFlightFences[currentFrame]);

    // Submit the queue, also receives the signal of finished a frame
    VkQueue& graphicsQueue = *device->getGraphicsQueue();
    if (vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo = {};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    // Which semaphores to wait
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    // Specify swapchains and the image
    VkSwapchainKHR& swapChain = *swapchain->getSwapChain();
    VkSwapchainKHR swapChains[] = { swapChain };
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    // Submits the request to present an image to swap chain
    VkQueue& presentQueue = *device->getPresentQueue();
    result = vkQueuePresentKHR(presentQueue, &presentInfo);
    // The VK_ERROR_OUT_OF_DATE_KHR is not guaranteed to happen
    // If result is VK_ERROR_OUT_OF_DATE_KHR or VK_SUBOPTIMAL_KHR or the frame buffer is resized, recreate the swap chain
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized) {
        // As the window is resized, framebufferResized is returned to false
        framebufferResized = false;
        recreateSwapChain();
    }
    else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }
    int MAX_FRAMES_IN_FLIGHT = semaphores->MAX_FRAMES_IN_FLIGHT;
    currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void Engine::mainProgram::recreateSwapChain()
{
    // This part for handling minimizing the window or the height = 0
    // If one of them is 0, wait until the window is resized to at least 1x1
    int width = 0, height = 0;
    while (width == 0 || height == 0) {
        glfwGetFramebufferSize(*window->getWindow(), &width, &height);
        glfwWaitEvents();
    }

    vkDeviceWaitIdle(*device->getLogicalDevice()); // Device could be in-use, wait for it to release

    cleanupSwapChain(); // Cleanup the old swapchain

    // Recreate the entire swap chain
    swapchain = new SwapChain(mainProgramPtr);
    renderPass = new Engine::RenderPass(mainProgramPtr);
    pipeline = new Engine::Pipeline(mainProgramPtr);
    
    image->createColorResources();
    image->createDepthResources();
    frameBuffer = new Engine::FrameBuffer(mainProgramPtr);
    model->createUniformBuffers();
    descriptorPool = new DescriptorPool(mainProgramPtr);
    descriptorSet->createDescriptorSets();
    commandBuffer->createCommandBuffers();

}

void Engine::mainProgram::keyHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == 1 || action == 2) {
        switch (key) {
        case GLFW_KEY_W:
            keys.W.press = true;
            break;
        case GLFW_KEY_A:
            keys.A.press = true;
            break;
        case GLFW_KEY_S:
            keys.S.press = true;
            break;
        case GLFW_KEY_D:
            keys.D.press = true;
            break;
        case GLFW_KEY_G:
            keys.G.press = true;
            break;
        case GLFW_KEY_B:
            keys.B.press = true;
            break;
        case GLFW_KEY_T:
            keys.T.press = true;
            break;
        case GLFW_KEY_0:
            keys.zero.press = true;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            keys.subtract.press = true;
            //changeMSAA("-");
            break;
        case GLFW_KEY_KP_ADD:
            keys.add.press = true;
            //changeMSAA("+");
            break;
        }
    }
    else {
        switch (key) {
        case GLFW_KEY_W:
            keys.W.press = false;
            break;
        case GLFW_KEY_A:
            keys.A.press = false;
            break;
        case GLFW_KEY_S:
            keys.S.press = false;
            break;
        case GLFW_KEY_D:
            keys.D.press = false;
            break;
        case GLFW_KEY_G:
            keys.G.press = false;
            break;
        case GLFW_KEY_B:
            keys.B.press = false;
            break;
        case GLFW_KEY_T:
            keys.T.press = false;
            break;
        case GLFW_KEY_0:
            keys.zero.press = false;
            break;
        case GLFW_KEY_KP_SUBTRACT:
            keys.subtract.press = false;
            break;
        case GLFW_KEY_KP_ADD:
            keys.add.press = false;
            break;
        }
    }
}


int main() {
//	Engine::mainProgram prog;
	Engine::mainProgram* prog;
	
	prog = new Engine::mainProgram();
	prog->init(&prog);

    prog->mainLoop();

    std::cout << "Program end!" << std::endl;
	return 0;
}
