#include "instance.h"
#include "debug.h"

namespace Engine {
    Instance::Instance(mainProgram** mainProgramPtr, VkInstance* instancePtr) {
        instance = instancePtr;
        // If validation layers are enabled but the requested validation layers are not available, throw an exception
        if (enableValidationLayers && !checkValidationLayerSupport()) {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        // Defines what is this application, optional
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // Defines this variable stores application infos
        appInfo.pApplicationName = "Vulkan Program"; // Application name
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        // Info of instance
        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO; // Defines this is the info of creating an instance
        createInfo.pApplicationInfo = &appInfo; // Uses application info appInfo with pointer

        // Extensions
        /*
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
        else
            std::cout << "Successfully created instance." << std::endl;
        */

        auto extensions = getRequiredExtensions();
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        /*
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr); // Retrieves number of extensions and store it to the pointer to variable in second parameter. First parameter is filter,
        std::vector<VkExtensionProperties> extensionsSupported(extensionCount); // Create a std::vector to store the extension details
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensionsSupported.data()); // Qeury extension details

        // Printing the list of available extensions
        std::cout << "Available extensions:" << std::endl;

        for (const auto& extensionList : extensionsSupported) {
            std::cout << "\t" << extensionList.extensionName << std::endl;
        }
        */

        // If validation layers are enabled, instance create info will have the numbers of enabled layers
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo;
        if (enableValidationLayers) {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledLayerNames = validationLayers.data();

            Debug::populateDebugMessengerCreateInfo(debugCreateInfo);
            createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else {
            // Set it to 0 if validation layers are disabled
            createInfo.enabledLayerCount = 0;

            createInfo.pNext = nullptr;
        }

        // Create the instance with the createInfo, nullptr should be a pointer to custom allocator callbacks, &instance is our instance that stores the handle to new object
        if (vkCreateInstance(&createInfo, nullptr, instance) != VK_SUCCESS) { // Pretty strainght forward enough, I shouldn't be too dumb to misunderstand it
            throw std::runtime_error("failed to create instance!"); // Throw an exception when failed to create an instance
        }
    }
    Instance::~Instance() {

    }

    // Return the required list of extensions
    std::vector<const char*> Instance::getRequiredExtensions() {
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;

        // We need an extension to interface with the window with GLFW in-built function to return the extnesions required
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        std::vector<const char*> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

        if (enableValidationLayers) {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        return extensions;
    }

    // Checks if all of the requested layers are available
    bool Instance::checkValidationLayerSupport() {
        // For storing the layer count
        uint32_t layerCount;
        // List all the available layers
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        // Create a vector to store the available layers
        std::vector<VkLayerProperties> availableLayers(layerCount);
        // Store all available layers to the vector availableLayers by enumerating
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
        // Check if all layers in validationLayers exist in availableLayers
        for (const char* layerName : validationLayers) {
            bool layerFound = false;
            // Loop through available layers, if the layer is found, break out the loop and search for the next layer
            for (const auto& layerProperties : availableLayers) {
                if (strcmp(layerName, layerProperties.layerName) == 0) {
                    layerFound = true;
                    break;
                }
            }
            // If the layer is not found, return false
            if (!layerFound)
                return false;
        }
        // If all required layers are found, return true
        return true;
    }
}