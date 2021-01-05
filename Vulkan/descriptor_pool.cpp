#include "descriptor_pool.h"

Engine::Descriptor_Pool::Descriptor_Pool(mainProgram** mainProgPtr)
{   
    mainProg = mainProgPtr;
    std::vector<VkImage> swapChainImages = *(*mainProg)->swapchain->getSwapChainImages();
    // Descriptor sets cannot be created directly, need pool for creating sets
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    // Descriptor count == number of images in swap chain
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(swapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();

    // Max num of descriptor sets may be allocated
    poolInfo.maxSets = static_cast<uint32_t>(swapChainImages.size());

    if (vkCreateDescriptorPool(*(*mainProg)->device->getLogicalDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}
