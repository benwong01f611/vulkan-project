#include "image.h"

Engine::Image::Image(mainProgram** mainProgramPtr)
{
	mainProg = mainProgramPtr;
    device = (*mainProg)->device->getLogicalDevice();
}

void Engine::Image::createColorResources()
{
	VkFormat colorFormat = *(*mainProg)->swapchain->getSwapChainImageFormat();
	VkExtent2D swapChainExtent = *(*mainProg)->swapchain->getSwapChainExtent();
	VkSampleCountFlagBits msaaSamples = *((*mainProg)->device->getMSAASamples(false));
	createImage(swapChainExtent.width, swapChainExtent.height, 1, *((*mainProg)->device->getMSAASamples(false)), colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
	colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
}

VkImage Engine::Image::createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory)
{
    VkImageCreateInfo imageInfo{};
    imageInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    // 1D image is for storing an array of data or gradient
    // 2D image is for textures
    // 3D image is for storing voxel volumes
    imageInfo.imageType = VK_IMAGE_TYPE_2D;
    imageInfo.extent.width = width;
    imageInfo.extent.height = height;
    // How many pixels there are on each axis, so depth = 1 
    imageInfo.extent.depth = 1;
    // Mipmapping
    imageInfo.mipLevels = mipLevels;
    imageInfo.arrayLayers = 1;

    imageInfo.format = format;

    // VK_IMAGE_TILING_LINEAR: Texels are laid out in row-major order like our pixels array
    // VK_IMAGE_TILING_OPTIMAL : Texels are laid out in an implementation defined order for optimal access
    // This cannot be changed later, so if we would like to directly access texels in the memory, we must use LINEAR
    imageInfo.tiling = tiling;

    // VK_IMAGE_LAYOUT_UNDEFINED: Not usable by the GPU and the very first transition will discard the texels.
    //VK_IMAGE_LAYOUT_PREINITIALIZED : Not usable by the GPU, but the first transition will preserve the texels.
    imageInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    // This image is used as destination for buffer copy, so it should be setup as transfer destination
    // Also we need it to be accessed from shader to color the mesh, so include SAMPLED BIT
    imageInfo.usage = usage;

    // Only used by 1 queue family supports graphics
    imageInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    // Multisampling
    imageInfo.samples = numSamples;
    imageInfo.flags = 0; // Optional

    // Create image
    if (vkCreateImage(*device, &imageInfo, nullptr, &image) != VK_SUCCESS) {
        throw std::runtime_error("failed to create image!");
    }

    VkMemoryRequirements memRequirements;
    vkGetImageMemoryRequirements(*device, image, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = (*mainProg)->memory->findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(*device, &allocInfo, nullptr, &imageMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate image memory!");
    }

    vkBindImageMemory(*device, image, imageMemory, 0);
}

VkImageView* Engine::Image::getColorImageView()
{
	return &colorImageView;
}

VkImageView Engine::Image::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels)
{
    // No viewInfo.components since VK_COMPONENT_SWIZZLE_IDENTITY is defined as 0
    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;
    if (vkCreateImageView(*device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

VkImage* Engine::Image::getColorImage()
{
    return &colorImage;
}

VkDeviceMemory* Engine::Image::getColorImageMemory()
{
    return &colorImageMemory;
}
