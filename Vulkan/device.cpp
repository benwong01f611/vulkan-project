#include "device.h"
#include "swapchain.h"
#include <vector>
#include <iostream>
#include <optional>
#include <set>

/*VkInstance instance;
std::vector<const char*> validationLayers;
bool enableValidationLayers;*/

const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct Engine::Device::QueueFamilyIndices {
    // Contains no value until assigning a value to it
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

void Engine::Device::pickPhysicalDevice()
{
    // Number of device
    uint32_t deviceCount = 0;
    // Find the number of devices (GPU) available on host computer
    vkEnumeratePhysicalDevices(*instance, &deviceCount, nullptr);
    // If no device supports vulkan, throw an exception
    if (deviceCount == 0) throw std::runtime_error("Failed to find a GPU with Vulkan support.");
    // Create an vector to hold all available devices
    std::vector<VkPhysicalDevice> devices(deviceCount);
    // Store the devices to the vector
    vkEnumeratePhysicalDevices(*instance, &deviceCount, devices.data());
    // If the device is suitable for using, select it
    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) {
            physicalDevice = device;
            maxMSAASamples = getMaxUsableSampleCount();
            msaaSamples = maxMSAASamples;
            break;
        }
    }

    // If the device support Vulkan but not suitable, throw an exception
    if (physicalDevice == VK_NULL_HANDLE) {
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
}
Engine::Device::Device(mainProgram** mainProgramPtr)
{
    mainProg = mainProgramPtr;
    instance = (*mainProg)->instance->getInstance();
    surface = (*mainProg)->surface->getSurface();


    pickPhysicalDevice();
    // This index is the physical device's queue index
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    // For presentation queue
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

    // Priority of the queue, for scheduling of command buffer execution, required even there is only one queue
    float queuePriority = 1.0f;
    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        // Add the create info to the last element of the create info vector
        queueCreateInfos.push_back(queueCreateInfo);
    }

    // Specifying the device features used
    VkPhysicalDeviceFeatures deviceFeatures = {};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    deviceFeatures.sampleRateShading = VK_TRUE;

    VkDeviceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;

    // Convert to uint32_t from the value obtained by size()
    // And point the data to queueCreateInfos
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    // Set the enabled features to the pointer to device features
    createInfo.pEnabledFeatures = &deviceFeatures;

    // Number of device extensions
    createInfo.enabledExtensionCount = static_cast<uint32_t>(deviceExtensions.size());
    // Extensions name
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    // If validation layers are enabled, layers will be set
    if ((*mainProg)->debugger->enableValidationLayers) {
        createInfo.enabledLayerCount = static_cast<uint32_t>((*mainProg)->debugger->validationLayers.size());
        createInfo.ppEnabledLayerNames = (*mainProg)->debugger->validationLayers.data();
    }
    else {
        createInfo.enabledLayerCount = 0;
    }
    // Create device, if failed then throw an exception
    if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &logicalDevice) != VK_SUCCESS) {
        throw std::runtime_error("failed to create logical device!");
    }
    // Retrieve queue handles with vkGetDeviceQueue
    // If the queue families are the same, the two handles will most likely have the same value
    vkGetDeviceQueue(logicalDevice, indices.graphicsFamily.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(logicalDevice, indices.presentFamily.value(), 0, &presentQueue);
}
Engine::Device::~Device()
{
    //vkDestroyCommandPool(logicalDevice, *CommandPool::getCommandPool(), nullptr); // Destroy command pool

    // Destroy the logical device
    vkDestroyDevice(logicalDevice, nullptr);
}
bool Engine::Device::isDeviceSuitable(VkPhysicalDevice device)
{
    /*vkGetPhysicalDeviceProperties(device, &deviceProperties);
    vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        deviceFeatures.geometryShader;
    */
    /*
    return true;
    */
    QueueFamilyIndices indices = findQueueFamilies(device);
    bool extensionsSupported = checkDeviceExtensionSupport(device);

    // Verify swap chain is adequate (sufficient)
    bool swapChainAdequate = false;
    if (extensionsSupported) {
        SwapChain* tempSwapChain = new SwapChain(mainProg, true);
        SwapChainSupportDetails swapChainSupport = tempSwapChain->querySwapChainSupport(device);
        delete tempSwapChain;
        // If the swapChainSupport formats and presentModes are NOT empty, swapChainAdequate will be true
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    // Get supported features
    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);

    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}
VkSampleCountFlagBits Engine::Device::getMaxUsableSampleCount()
{
    VkPhysicalDeviceProperties physicalDeviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &physicalDeviceProperties);

    // Get max sample count
    VkSampleCountFlags counts = physicalDeviceProperties.limits.framebufferColorSampleCounts & physicalDeviceProperties.limits.framebufferDepthSampleCounts;
    if ((*mainProg)->debugger->enableValidationLayers)
        std::cout << "Max sample (MSAA): " << counts << std::endl;
    if (counts & VK_SAMPLE_COUNT_64_BIT) { return VK_SAMPLE_COUNT_64_BIT; }
    if (counts & VK_SAMPLE_COUNT_32_BIT) { return VK_SAMPLE_COUNT_32_BIT; }
    if (counts & VK_SAMPLE_COUNT_16_BIT) { return VK_SAMPLE_COUNT_16_BIT; }
    if (counts & VK_SAMPLE_COUNT_8_BIT) { return VK_SAMPLE_COUNT_8_BIT; }
    if (counts & VK_SAMPLE_COUNT_4_BIT) { return VK_SAMPLE_COUNT_4_BIT; }
    if (counts & VK_SAMPLE_COUNT_2_BIT) { return VK_SAMPLE_COUNT_2_BIT; }

    return VK_SAMPLE_COUNT_1_BIT;
}

Engine::Device::QueueFamilyIndices Engine::Device::findQueueFamilies(VkPhysicalDevice device)
{
    Engine::Device::QueueFamilyIndices indices;
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());
    int i = 0;
    // Find at least 1 queue that supports VK_QUEUE_GRAPHICS_BIT
    for (const auto& queueFamily : queueFamilies) {
        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.graphicsFamily = i;
        }
        // If the queue family is the one that supports presentation support, set the present family index to i (index of queue family)
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentSupport);
        if (presentSupport)
            indices.presentFamily = i;
        if (indices.isComplete()) break;
        i++;
    }
    // Assign index to queue families that could be found
    return indices;
}
bool Engine::Device::checkDeviceExtensionSupport(VkPhysicalDevice device)
{
    // Get the number of extensions supported by GPU
    uint32_t extensionCount;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    // Available extensions of GPU
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    // Get the name of all required extensions
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

    // Check whether the required extension(s) is/are supported by the device
    for (const auto& extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }
    // If it is empty, it means the device fully supports the application
    return requiredExtensions.empty();
}

VkSampleCountFlagBits* Engine::Device::getMSAASamples(bool isMax)
{
    if (isMax)
        return &maxMSAASamples;
    else
        return &msaaSamples;

}

VkPhysicalDevice* Engine::Device::getPhysicalDevice()
{
    return &physicalDevice;
}

VkDevice* Engine::Device::getLogicalDevice()
{
    return &logicalDevice;
}

VkQueue* Engine::Device::getGraphicsQueue()
{
    return &graphicsQueue;
}
