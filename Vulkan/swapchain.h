#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "main.h"

/*VkSwapchainKHR swapChain;
std::vector<VkImage> swapChainImages;
VkFormat swapChainImageFormat;
VkExtent2D swapChainExtent;
GLFWwindow* window;*/

namespace Engine {
    // Swap chain details
    // Contains few things:
    //      1. Basic surface capabilities (min/max number of images in swap chain, min/max width and height of images
    //      2. Surface formats (Pixel format, color space)
    //      3. Available presentation modes
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };
    
    class SwapChain {
    public:
        SwapChain(mainProgram** mainProgramPtr);
        SwapChain(mainProgram** mainProgramPtr, bool isTemp);
        ~SwapChain();
        SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
        VkSwapchainKHR* getSwapChain();
        std::vector<VkImage>* getSwapChainImages();
        VkFormat* getSwapChainImageFormat();
        VkExtent2D* getSwapChainExtent();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);

    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews; // Store image views
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        VkDevice* logicalDevice;
        void createImageViews();
        mainProgram** mainProg;
        VkPhysicalDevice* physicalDevice;
    };
}