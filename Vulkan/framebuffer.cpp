#include "framebuffer.h"
#include <array>
Engine::FrameBuffer::FrameBuffer(mainProgram** mainProgramPtr)
{
    mainProg = mainProgramPtr;
    std::vector<VkImageView> swapChainImageViews = *((*mainProg)->swapchain->getSwapChainImageViews());
    swapChainFramebuffers.resize(swapChainImageViews.size()); // Resize the vector to hold all framebuffers
        // Iterate through all image views and create framebuffers from them
    for (size_t i = 0; i < swapChainImageViews.size(); i++) {
        std::array<VkImageView, 3> attachments = {
            *(*mainProg)->image->getColorImageView(),
            * (*mainProg)->image->getDepthImageView(),
            swapChainImageViews[i]
        };

        // Framebuffer create info
        VkFramebufferCreateInfo framebufferInfo = {};
        framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
        framebufferInfo.renderPass = *(*mainProg)->renderPass->getRenderPass(); // Specify with which render pass the framebuffer needs to be compatible
        framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size()); // How many attachment should be used in the attachment array
        framebufferInfo.pAttachments = attachments.data();
        // Size of the framebuffer
        VkExtent2D swapChainExtent = *(*mainProg)->swapchain->getSwapChainExtent();
        framebufferInfo.width = swapChainExtent.width;
        framebufferInfo.height = swapChainExtent.height;
        framebufferInfo.layers = 1; // Swap chain images are single images, so layers = 1

        // Create framebuffer
        if (vkCreateFramebuffer(*(*mainProg)->device->getLogicalDevice(), &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS) {
            throw std::runtime_error("failed to create framebuffer!");
        }
    }
}

Engine::FrameBuffer::~FrameBuffer()
{
    // Destroy all framebuffers in swap chain
    for (auto framebuffer : swapChainFramebuffers) {
        vkDestroyFramebuffer(*(*mainProg)->device->getLogicalDevice(), framebuffer, nullptr);
    }
}

std::vector<VkFramebuffer>* Engine::FrameBuffer::getSwapChainFramebuffers()
{
    return &swapChainFramebuffers;
}
