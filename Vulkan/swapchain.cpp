#include "swapchain.h"
#include "device.h"
#include <iostream>
#include <optional>

namespace Engine {
    struct Device::QueueFamilyIndices {
        // Contains no value until assigning a value to it
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    SwapChain::SwapChain(mainProgram** mainProgramPtr)
    {
        mainProg = mainProgramPtr;
        physicalDevice = (*mainProg)->device->getPhysicalDevice();
        logicalDevice = (*mainProg)->device->getLogicalDevice();
        // Query swap chain support from physical device
        SwapChainSupportDetails swapChainSupport = querySwapChainSupport(*physicalDevice);

        // Swap chain details
        VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
        VkExtent2D extent = chooseSwapExtent(swapChainSupport.capabilities);

        // How many images we would like to have in swap chain
        // + 1 is to prevent the waiting on the driver to complete internal oeprations before we can acquire another image to render to
        uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
        // Not to exceed the maximum number of images, if maxImageCount == 0, it means that it has no maximum image count (infinity)
        if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
            imageCount = swapChainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = *(*mainProg)->surface->getSurface();

        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = surfaceFormat.format;
        createInfo.imageColorSpace = surfaceFormat.colorSpace;
        createInfo.imageExtent = extent;
        // Always 1 except developing stereoscopic 3D application
        createInfo.imageArrayLayers = 1;
        // Render directly to them
        // VK_IMAGE_USAGE_TRANSFer_DST_BIT is to render images to a separate image first to perfom operations like post-processing and use memory operation to transfer the rendered iamge to swap chain image
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        // Handle swap chain images that will be used across multiple queue families
        Device::QueueFamilyIndices indices = (*mainProg)->device->findQueueFamilies(*physicalDevice);
        uint32_t queueFamilyIndices[] = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        if (indices.graphicsFamily != indices.presentFamily) {
            // An image is owned by one queue family at a time and ownership must be explicitly transfered before using it in another queue family. This option offers the best performance.
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else {
            // Images can be used across multiple queue families without explicit ownership transfers.
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0; // Optional
            createInfo.pQueueFamilyIndices = nullptr; // Optional
        }

        // Specifying certain transform should be applied to the images in the swap chain if supported
        // currentTransform means no transform
        createInfo.preTransform = swapChainSupport.capabilities.currentTransform;
        // Specifies if the alpha channel should be used for blending with other windows in the window system, almost always want to ignore
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;

        createInfo.presentMode = presentMode;
        // Don't care about the color of pixels that are obscured (dim?), enabling clipping gets the best performance, disable to read these pixels blocked by other windows to get predictable results
        createInfo.clipped = VK_TRUE;

        // Old swap chain
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        // Create swap chain
        if (vkCreateSwapchainKHR(*logicalDevice, &createInfo, nullptr, &swapChain) != VK_SUCCESS) {
            throw std::runtime_error("failed to create swap chain!");
        }
        // Query the final number of images and resize the container and retrieve handles
        vkGetSwapchainImagesKHR(*logicalDevice, swapChain, &imageCount, nullptr);
        swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(*logicalDevice, swapChain, &imageCount, swapChainImages.data());

        // Store them in global variables for future use
        swapChainImageFormat = surfaceFormat.format;
        swapChainExtent = extent;
    }
    SwapChain::SwapChain(mainProgram** mainProgramPtr, bool isTemp)
    {
        mainProg = mainProgramPtr;
    }
    SwapChain::~SwapChain()
    {
    }
    SwapChainSupportDetails SwapChain::querySwapChainSupport(VkPhysicalDevice device)
    {
        VkSurfaceKHR* surfacePtr = (*mainProg)->surface->getSurface();
        SwapChainSupportDetails details;
        // Takes specified physicalDevice and surface to query the surface capabilities
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, *surfacePtr, &details.capabilities);

        // Querying the supported surface formats
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, *surfacePtr, &formatCount, nullptr);

        if (formatCount != 0) {
            // Resize the vector to hold all availiable formats with the size of the format
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, *surfacePtr, &formatCount, details.formats.data());
        }

        // Querying supported presentation modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, *surfacePtr, &presentModeCount, nullptr);

        if (presentModeCount != 0) {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, *surfacePtr, &presentModeCount, details.presentModes.data());
        }
        return details;
    }
    VkSwapchainKHR* SwapChain::getSwapChain()
    {
        return &swapChain;
    }
    std::vector<VkImage>* SwapChain::getSwapChainImages()
    {
        return &swapChainImages;
    }
    VkFormat* SwapChain::getSwapChainImageFormat()
    {
        return &swapChainImageFormat;
    }
    VkExtent2D* SwapChain::getSwapChainExtent()
    {
        return &swapChainExtent;
    }
    std::vector<VkImageView>* SwapChain::getSwapChainImageViews()
    {
        return &swapChainImageViews;
    }
    VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats)
    {
        // Set the current format to SRGB for now
        for (const auto& availableFormat : availableFormats) {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
                return availableFormat;
            }
        }

        return availableFormats[0];
    }
    VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes)
    {
        // MAILBOX_KHR is better
        // IMMEDIATE_KHR:       Directly submitted to the screen, may result in tearing
        // FIFO_KHR:            VSync, when the display is refreshed, it is called 'vertical blank'
        // FIFO_RELAXED_KHR:    Nearly same as FIFO_KHR, but if the application is late and the queue is empty at last vertical blank, the image will be transferred to the display but not waiting next vertical blank, may result tearing
        // MAILBOX_KHR:         Queued image will be replaced to newer one, allow implementing triple buffering, avoid teraing with significantly less latency issues than standard VSync that uses double buffering
        for (const auto& availablePresentMode : availablePresentModes) {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
                return availablePresentMode;
            }
            // If MAILBOX_KHR isn't available, use FIFO instead
            return VK_PRESENT_MODE_FIFO_KHR;
        }
    }
    VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
    {
        if (capabilities.currentExtent.width != UINT32_MAX) {
            return capabilities.currentExtent;
        }
        else {
            int width, height;
            glfwGetFramebufferSize(*(*mainProg)->window->getWindow(), &width, &height);

            VkExtent2D actualExtent = {
                static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)
            };

        }
    }
    void SwapChain::createImageViews() {
        // Resize the list to fit all image views
        swapChainImageViews.resize(swapChainImages.size());

        // Iterates all swap chain images
        for (uint32_t i = 0; i < swapChainImages.size(); i++) {
            swapChainImageViews[i] = createImageView(swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
        }
    }
    VkImageView SwapChain::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {
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
        if (vkCreateImageView(*logicalDevice, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
            throw std::runtime_error("failed to create texture image view!");
        }

        return imageView;
    }
}

