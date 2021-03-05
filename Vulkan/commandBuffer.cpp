#include "commandBuffer.h"

Engine::CommandBuffer::CommandBuffer(   Device& deviceRef,
                                        SwapChain& swapChainRef, 
                                        RenderPass& renderPassRef, 
                                        CommandPool& commandPoolRef, 
                                        FrameBuffer& frameBufferRef, 
                                        Pipeline& pipelineRef, 
                                        Model& modelRef, 
                                        DescriptorSet& descriptorSetRef) : 
                                            device(deviceRef), 
                                            swapChain(swapChainRef), 
                                            renderPass(renderPassRef), 
                                            commandPool(commandPoolRef), 
                                            frameBuffer(frameBufferRef), 
                                            pipeline(pipelineRef), 
                                            model(modelRef),
                                            descriptorSet(descriptorSetRef)
{
}

Engine::CommandBuffer::~CommandBuffer() {
    destroyCommandBuffers();
}

VkCommandBuffer Engine::CommandBuffer::beginSingleTimeCommands()
{
    // This function is for copying the staging buffer to vertex buffer
        // Create temporary command buffer
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool.getCommandPool();
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(device.getLogicalDevice(), &allocInfo, &commandBuffer);

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
    vkQueueSubmit(device.getGraphicsQueue(), 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(device.getGraphicsQueue());

    vkFreeCommandBuffers(device.getLogicalDevice(), commandPool.getCommandPool(), 1, &commandBuffer);
}



void Engine::CommandBuffer::createCommandBuffers()
{
    std::vector<VkFramebuffer> swapChainFramebuffers = frameBuffer.getSwapChainFramebuffers();
    commandBuffers.resize(swapChainFramebuffers.size()); // Resize the command buffers size according to the size of the swap chain
    // Command buffers are allocated with vkAllocateCommandBuffers, and takes VkCommandBufferAllocateInfo struct to specify the command pool and numbers of buffers to allocate
    VkCommandBufferAllocateInfo allocInfo = {};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool.getCommandPool();
    // PRIMARY: Can be submitted to a queue for execution, but cannot be called from other command buffers
    // SECONDARY: Cannot be submitted directly, but can be called from primary command buffers.
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    // Numbers of command buffers
    allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

    // Allocate command buffers
    if (vkAllocateCommandBuffers(device.getLogicalDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }

    // Record command buffer by calling vkBeginCommandBuffer with info
    for (size_t i = 0; i < commandBuffers.size(); i++) {
        VkCommandBufferBeginInfo beginInfo = {};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        /*
        VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT: The command buffer will be rerecorded right after executing it once.
        VK_COMMAND_BUFFER_USAGE_RENDER_PASS_CONTINUE_BIT : This is a secondary command buffer that will be entirely within a single render pass.
        VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT : The command buffer can be resubmitted while it is also already pending execution.
        */
        //beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT; // Optional
        beginInfo.pInheritanceInfo = nullptr; // Optional, for secondary command buffers

                                              // Record
        if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        // Drawing starts by render pass
        // Render pass info
        VkRenderPassBeginInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = renderPass.getRenderPass();
        renderPassInfo.framebuffer = swapChainFramebuffers[i]; // Create framebuffer for each swap chain image
        renderPassInfo.renderArea.offset = { 0, 0 };
        renderPassInfo.renderArea.extent = swapChain.getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f }; // 100% opacity
        clearValues[1].depthStencil = { 1.0f, 0 };

        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        // Begin render pass
        /*
        VK_SUBPASS_CONTENTS_INLINE: The render pass commands will be embedded in the primary command buffer itself and no secondary command buffers will be executed.
        VK_SUBPASS_CONTENTS_SECONDARY_COMMAND_BUFFERS: The render pass commands will be executed from secondary command buffers.

        */
        vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        // Begin drawing
        vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getGraphicsPipeline());

        // Bind vertex buffers to bindings
        VkBuffer vertexBuffers[] = { model.getVertexBuffers() };
        VkDeviceSize offsets[] = { 0 };
        vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
        // indexType should be uint_32 since there are lots of vertices to load
        vkCmdBindIndexBuffer(commandBuffers[i], model.getIndexBuffers(), 0, VK_INDEX_TYPE_UINT32);
        // Bind descriptor sets to command buffer
        vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.getPipelineLayout(), 0, 1, &(descriptorSet.getDescriptorSets()[i]), 0, nullptr);

        /*
        vkCmdDraw(commandBuffers[i], vertexCount, instanceCount, firstVertex, firstInstance);
        vertexCount: Even though we don't have a vertex buffer, we technically still have 3 vertices to draw.
        instanceCount : Used for instanced rendering, use 1 if you're not doing that.
        firstVertex : Used as an offset into the vertex buffer, defines the lowest value of gl_VertexIndex.
        firstInstance : Used as an offset for instanced rendering, defines the lowest value of gl_InstanceIndex.
        */
        //vkCmdDraw(commandBuffers[i], static_cast<uint32_t>(vertices.size()), 1, 0, 0);

        vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(model.indices.size()), 1, 0, 0, 0);
        // End drawing
        vkCmdEndRenderPass(commandBuffers[i]);

        // End the recording of command buffer
        if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to record command buffer!");
        }
    }
}

std::vector<VkCommandBuffer>* Engine::CommandBuffer::getCommandBuffers()
{
    return &commandBuffers;
}

void Engine::CommandBuffer::destroyCommandBuffers()
{
    // Recreating command buffers is wasteful (?), which can reuse existing pool to allocate new command buffers
    // Like destroying the object requires time to recreate, so free up the content is much more efficient than destroying the object
    vkFreeCommandBuffers(device.getLogicalDevice(), commandPool.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
}
