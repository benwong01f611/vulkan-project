#include "renderpass.h"
#include <array>
Engine::RenderPass::RenderPass(mainProgram** mainProgramPtr)
{/* loadOp:
            VK_ATTACHMENT_LOAD_OP_LOAD: Preserve the existing contents of the attachment
            VK_ATTACHMENT_LOAD_OP_CLEAR: Clear the values to a constant at the start
            VK_ATTACHMENT_LOAD_OP_DONT_CARE: Existing contents are undefined; we don't care about them
           storeOp:
            VK_ATTACHMENT_STORE_OP_STORE: Rendered contents will be stored in memory and can be read later
            VK_ATTACHMENT_STORE_OP_DONT_CARE: Contents of the framebuffer will be undefined after the rendering operation
        */
    mainProg = mainProgramPtr;
    VkSampleCountFlagBits msaaSamples = *((*mainProg)->device->getMSAASamples(false));
    VkFormat swapChainImageFormat = *((*mainProg)->swapchain->getSwapChainImageFormat());
    VkAttachmentDescription colorAttachment = {};
    colorAttachment.format = swapChainImageFormat;
    colorAttachment.samples = msaaSamples;
    // These two apply to color and depth data
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    // These two apply to stencil data
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // Defines which layout the image will have before the render pass begins
    // UNDEFINED means don't care what previous layout the image was in
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    // COLOR_ATTACHMENT_OPTIMAL:    Images used as color attachment
    // PRESENT_SRC_KHR:             Images to be presented in the swap chain
    // TRANSFER_DST_OPTIMAL:        Images to be used as destination for a memory copy operation
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    // Single render pass can consist multiple subpasses
    // Subpasses are subsequent rendering operations that depend on the contents of framebuffers in previous passes
    VkAttachmentReference colorAttachmentRef = {};
    // Attachment index
    colorAttachmentRef.attachment = 0;
    // Which layout we would like the attachment to have during subpasses that uses this reference
    // Best performance
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    // Depth attachment
    VkAttachmentDescription depthAttachment{};
    depthAttachment.format = findDepthFormat();
    depthAttachment.samples = msaaSamples;
    depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    // We don't care about storing depth data
    depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    // We don't care about the previous depth content
    depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentReference depthAttachmentRef{};
    depthAttachmentRef.attachment = 1;
    depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

    VkAttachmentDescription colorAttachmentResolve{};
    colorAttachmentResolve.format = swapChainImageFormat;
    colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

    VkAttachmentReference colorAttachmentResolveRef{};
    colorAttachmentResolveRef.attachment = 2;
    colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    // DIrectly referenced from the fragment shader
    subpass.colorAttachmentCount = 1;
    /*
        pInputAttachments: Attachments that are read from a shader
        pResolveAttachments: Attachments used for multisampling color attachments
        pDepthStencilAttachment: Attachment for depth and stencil data
        pPreserveAttachments: Attachments that are not used by this subpass, but for which the data must be preserved
    */
    subpass.pColorAttachments = &colorAttachmentRef;
    subpass.pDepthStencilAttachment = &depthAttachmentRef;
    subpass.pResolveAttachments = &colorAttachmentResolveRef;

    // Specifies the indices of the dependency and the dependent subpass
    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL; // Refers to the inplicit subpass before or after the render pass depending on whether it is specified in srcSubpass or dstSubpass
    dependency.dstSubpass = 0; // Refers to the only subpass, always higher than srcSubpass
    // Operations to wait on and stages in which these operations occur
    // As we need to wait for the swapchain image to finish reading from the image before we can access it.
    // So we decided to wait for the color attachment output stage to complete
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    // Prevent transition from happenig unitl it is actually necessary and allowed
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    //dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

    std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment , colorAttachmentResolve };

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;
    renderPassInfo.dependencyCount = 1;
    renderPassInfo.pDependencies = &dependency;
    
    if (vkCreateRenderPass(*(*mainProg)->device->getLogicalDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
        throw std::runtime_error("failed to create render pass!");
    }

}
VkFormat Engine::RenderPass::findDepthFormat() {
    // S8_UINT contains stencil component
    return findSupportedFormat(
        { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
        VK_IMAGE_TILING_OPTIMAL,
        VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
    );
}

VkFormat Engine::RenderPass::findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
    for (VkFormat format : candidates) {
        // VkFormatProperties includes these 3 fields:
        // linearTilingFeatures: Use cases that are supported with linear tiling
        // optimalTilingFeatures : Use cases that are supported with optimal tiling
        // bufferFeatures : Use cases that are supported for buffers

        VkFormatProperties props;
        vkGetPhysicalDeviceFormatProperties(*(*mainProg)->device->getPhysicalDevice(), format, &props);
        if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
            return format;
        }
        else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
            return format;
        }
    }
    // If none of the candidates (GPU) can perform it, throw an exception
    throw std::runtime_error("failed to find supported format!");
}

VkRenderPass* Engine::RenderPass::getRenderPass()
{
    return &renderPass;
}

Engine::mainProgram** Engine::RenderPass::getParent()
{
    return mainProg;
}
