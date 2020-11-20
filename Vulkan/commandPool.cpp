#include "commandPool.h"
#include "device.h"
#include <optional>

struct Engine::Device::QueueFamilyIndices {
    // Contains no value until assigning a value to it
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

Engine::CommandPool::CommandPool(mainProgram** mainProgramPtr)
{
    mainProg = mainProgramPtr;
    
    // Command buffers are executed by submitting them on one of the device queues
    Engine::Device::QueueFamilyIndices queueFamilyIndices = (*mainProg)->device->findQueueFamilies(*(*mainProg)->device->getPhysicalDevice());

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
    // VK_COMMAND_POOL_CREATE_TRANSIENT_BIT: Hint that cmd buffers are rerecorded with new cmds very often (may change mem allocation behavior)
    // VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT: Allow command buffers to be rerecorded individually, without this flag they all have to be reset together
    poolInfo.flags = 0; // Optional
    // Create command pool
    if (vkCreateCommandPool(*(*mainProg)->device->getLogicalDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

VkCommandPool* Engine::CommandPool::getCommandPool()
{
    return &commandPool;
}
