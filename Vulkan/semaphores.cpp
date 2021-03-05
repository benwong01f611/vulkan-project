#include "semaphores.h"

Engine::Semaphores::Semaphores(Engine::SwapChain& swapChainref, Engine::Device& deviceref) : swapChain(swapChainref),device(deviceref)
{
    VkDevice& logicalDevice = device.getLogicalDevice();
    // Resizes the vector according to the MAX_FRAMES_IN_FLIGHT to match with it
    imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    imagesInFlight.resize(swapChain.getSwapChainImages().size(), VK_NULL_HANDLE);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    // Create semaphore and fence for each frame in swap chain
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        if (vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(logicalDevice, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(logicalDevice, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create synchronization objects for a frame!");
        }
    }
}

Engine::Semaphores::~Semaphores()
{
    VkDevice& logicalDevice = device.getLogicalDevice();
    // Destroy semaphore and fence for all frames
    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        vkDestroySemaphore(logicalDevice, renderFinishedSemaphores[i], nullptr);
        vkDestroySemaphore(logicalDevice, imageAvailableSemaphores[i], nullptr);
        vkDestroyFence(logicalDevice, inFlightFences[i], nullptr);
    }
}

std::vector<VkSemaphore>& Engine::Semaphores::getImageAvailableSemaphores()
{
    return imageAvailableSemaphores;
}

std::vector<VkSemaphore>& Engine::Semaphores::getRenderFinishedSemaphores()
{
    return renderFinishedSemaphores;
}

std::vector<VkFence>& Engine::Semaphores::getInFlightFences()
{
    return inFlightFences;
}

std::vector<VkFence>& Engine::Semaphores::getImagesInFlight()
{
    return imagesInFlight;
}
