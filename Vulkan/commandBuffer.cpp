#include "commandBuffer.h"

Engine::CommandBuffer::CommandBuffer(mainProgram** mainProgramPtr)
{
	mainProg = mainProgramPtr;
}

VkCommandBuffer Engine::CommandBuffer::beginSingleTimeCommands()
{
    // This function is for copying the staging buffer to vertex buffer
        // Create temporary command buffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = *(*mainProg)->commandPool->getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(*(*mainProg)->device->getLogicalDevice(), &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    // As we are going to use the command buffer once only, use VK_COMMAND_BUFFER_USAGE_ONE_TIE_SUBMIT_BIT
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    return commandBuffer;
}

void Engine::CommandBuffer::copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height)
{
    VkCommandBuffer commandBuffer = beginSingleTimeCommands();

    VkBufferImageCopy region{};
    region.bufferOffset = 0;
    region.bufferRowLength = 0;
    region.bufferImageHeight = 0;
    // Which part of the image we want to copy the pixels
    region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    region.imageSubresource.mipLevel = 0;
    region.imageSubresource.baseArrayLayer = 0;
    region.imageSubresource.layerCount = 1;

    region.imageOffset = { 0, 0, 0 };
    region.imageExtent = {
        width,
        height,
        1
    };

    // Copy the image
    vkCmdCopyBufferToImage(
        commandBuffer,
        buffer,
        image,
        VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
        1,
        &region
    );

    endSingleTimeCommands(commandBuffer);
}

void Engine::CommandBuffer::endSingleTimeCommands(VkCommandBuffer commandBuffer)
{
    // This cmd buffer only contains copy command
    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    // Submit the command buffer
    vkQueueSubmit(*(*mainProg)->device->getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(*(*mainProg)->device->getGraphicsQueue());

    vkFreeCommandBuffers(*(*mainProg)->device->getLogicalDevice(), *(*mainProg)->commandPool->getCommandPool(), 1, &commandBuffer);
}
