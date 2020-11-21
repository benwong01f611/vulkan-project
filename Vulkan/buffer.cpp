#include "buffer.h"

Engine::Buffer::Buffer(mainProgram** mainProgramPtr)
{
	mainProg = mainProgramPtr;
}

void Engine::Buffer::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    // Set to exclusive if the image will not be shared to others
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    VkDevice* device = (*mainProg)->device->getLogicalDevice();

    if (vkCreateBuffer(*device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    // Get available tpes of memory
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(*device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    // Allocation size is equal to the memory size requirement
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = (*mainProg)->memory->findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(*device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    // Associate memory with buffer, 0 is offset, if it is non-zero, it must be divisible by memRequirements.alignment
    vkBindBufferMemory(*device, buffer, bufferMemory, 0);
}

void Engine::Buffer::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = (*mainProg)->commandBuffer->beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    // Contents of buffer is transferred by vkCmdCopyBuffer, 1 means the region count
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    (*mainProg)->commandBuffer->endSingleTimeCommands(commandBuffer);
}
