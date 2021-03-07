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
    class Device;
    class Surface;
    class Window;
    class SwapChain {
    public:
        SwapChain(Device& device, Surface& surface, Window& window);
        ~SwapChain();
        static SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
        VkSwapchainKHR& getSwapChain();
        std::vector<VkImage>& getSwapChainImages();
        VkFormat& getSwapChainImageFormat();
        VkExtent2D& getSwapChainExtent();
        std::vector<VkImageView>& getSwapChainImageViews();
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
        void createImageViews();
        void destroyImageViews();
        void initSwapChain();

    private:
        bool isSwapChainTemp;
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        std::vector<VkImageView> swapChainImageViews; // Store image views
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        Surface& surface;
        Device& device;
        Window& window;
    };
}