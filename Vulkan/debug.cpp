#include "debug.h"
#include "main.h"


Engine::Debug::Debug(Instance& instanceRef) : instanceLocalRef(instanceRef), instance(instanceLocalRef.getInstance())
{
}

Engine::Debug::~Debug()
{
    if (!enableValidationLayers) return;
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        auto pAllocator = nullptr;
        func(instance, Debug::debugMessenger, pAllocator);
    }
}

void Engine::Debug::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo)
{
    createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr; // Optional
}

void Engine::Debug::initDebug()
{
    instance = instanceLocalRef.getInstance();
    // If validation layers are disabled then we don't need to set it up
    if (!enableValidationLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo;
    populateDebugMessengerCreateInfo(createInfo);
    if (CreateDebugUtilsMessengerEXT(instance, &createInfo, nullptr, &(Debug::debugMessenger)) != VK_SUCCESS) {
        throw std::runtime_error("failed to set up debug messenger!");
    }
}

VKAPI_ATTR VkBool32 VKAPI_CALL Engine::Debug::debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{// messageSeverity:
    //      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:        Diagnostic message
    //      VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:           Informational message like the creation of a resource
    //      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:        Message about behavior that is not necessarily an error, but very likely a bug in your application
    //      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:          Message about behavior that is invalid and may cause crashes
    // messageType:
    //      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:            Some event has happened that is unrelated to the specification or performance
    //      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:         Something has happened that violates the specification or indicates a possible mistake
    //      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:        Potential non-optimal use of Vulkan 
    // pCallbackData: Details of message
    //      pMessage:       The debug message as a null-terminated string
    //      pObjects:       Array of Vulkan object handles related to the message
    //      objectCount:    Number of objects in array


    std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

    return VK_FALSE;

}

VkResult Engine::Debug::CreateDebugUtilsMessengerEXT(VkInstance vkinstance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(vkinstance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(vkinstance, pCreateInfo, pAllocator, pDebugMessenger);
    }
    else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

