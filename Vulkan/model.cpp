#include "model.h"
#pragma once
// tinyobjloader for 3D models
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <chrono>

Engine::Model::Model(mainProgram** mainProgramPtr,KeyInput& keyInputref) : keyInput(keyInputref)
{
	mainProg = mainProgramPtr;
}

void Engine::Model::loadModel()
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, model_path.c_str()))
        throw std::runtime_error(warn + err);
    if ((*mainProg)->debugger->enableValidationLayers)
        std::cout << "[tinyobjloader] Warning: " << warn << std::endl << "[tinyobjloader] Error: " << err << std::endl;

    std::unordered_map<Vertex, uint32_t> uniqueVertices{};

    // For each shapes (faces)
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex{};

            vertex.pos = {
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            };

            vertex.texCoord = {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = { 1.0f, 1.0f, 1.0f };

            if (uniqueVertices.count(vertex) == 0) {
                uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }

            indices.push_back(uniqueVertices[vertex]);
        }
    }
}
bool Engine::Model::hasStencilComponent(VkFormat format) {
    return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
}


void Engine::Model::createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    // Set to exclusive if the image will not be shared to others
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    logicalDevice = (*mainProg)->device->getLogicalDevice();

    if (vkCreateBuffer(*logicalDevice, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create buffer!");
    }

    // Get available tpes of memory
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(*logicalDevice, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    // Allocation size is equal to the memory size requirement
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = (*mainProg)->memory->findMemoryType(memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(*logicalDevice, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate buffer memory!");
    }

    // Associate memory with buffer, 0 is offset, if it is non-zero, it must be divisible by memRequirements.alignment
    vkBindBufferMemory(*logicalDevice, buffer, bufferMemory, 0);
}

void Engine::Model::copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
{
    VkCommandBuffer commandBuffer = (*mainProg)->commandBuffer->beginSingleTimeCommands();

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    // Contents of buffer is transferred by vkCmdCopyBuffer, 1 means the region count
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    (*mainProg)->commandBuffer->endSingleTimeCommands(commandBuffer);
}

void Engine::Model::createVertexBuffer()
{// Purpose of data in vertex buffer: VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT

        // Size of vertex * number of vertices
    VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

    // Create staging buffer
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    // VK_BUFFER_USAGE_TRANSFER_SRC_BIT: use buffer as source in memory transfer
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    // Copy the vertex data to mapped memory and unmap it    
    void* data;
    // Second last parameter must be zero as no flag is currently not available by API
    // Last paramter is the output
    vkMapMemory(*logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, vertices.data(), (size_t)bufferSize);
    vkUnmapMemory(*logicalDevice, stagingBufferMemory);

    // Create vertex buffer
    // VK_BUFFER_USAGE_TRANSFER_DST_BIT: use buffer as destination in memory transfer
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

    copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(*logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(*logicalDevice, stagingBufferMemory, nullptr);
}

void Engine::Model::createIndexBuffer()
{// Same as vertex and staging buffer, but using the size and number of index but not vertex
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(*logicalDevice, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t)bufferSize);
    vkUnmapMemory(*logicalDevice, stagingBufferMemory);

    createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(stagingBuffer, indexBuffer, bufferSize);

    vkDestroyBuffer(*logicalDevice, stagingBuffer, nullptr);
    vkFreeMemory(*logicalDevice, stagingBufferMemory, nullptr);
}

void Engine::Model::createUniformBuffers()
{
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    std::vector<VkImage> swapChainImages = *(*mainProg)->swapchain->getSwapChainImages();
    uniformBuffers.resize(swapChainImages.size()); // Resize the uniform buffers to match with the size of swapchain
    uniformBuffersMemory.resize(swapChainImages.size());
    for (size_t i = 0; i < swapChainImages.size(); i++) {
        createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }
}

std::vector<VkBuffer>* Engine::Model::getUniformBuffers()
{
    return &uniformBuffers;
}

std::vector<VkDeviceMemory>* Engine::Model::getUniformBuffersMemory()
{
    return &uniformBuffersMemory;
}

VkBuffer* Engine::Model::getIndexBuffers()
{
    return &indexBuffer;
}

VkDeviceMemory* Engine::Model::getIndexBuffersMemory()
{
    return &indexBufferMemory;
}

VkBuffer* Engine::Model::getVertexBuffers()
{
    return &vertexBuffer;
}

VkDeviceMemory* Engine::Model::getVertexBuffersMemory()
{
    return &vertexBufferMemory;
}

void Engine::Model::destroyUniformBuffer()
{
    // Destroy uniform buffer
    for (size_t i = 0; i < (*(*mainProg)->swapchain->getSwapChainImages()).size(); i++) {
        vkDestroyBuffer(*logicalDevice, uniformBuffers[i], nullptr);
        vkFreeMemory(*logicalDevice, uniformBuffersMemory[i], nullptr);
    }
}

Engine::Model::~Model() {
    destroyUniformBuffer();

    vkDestroyBuffer(*logicalDevice, indexBuffer, nullptr);
    vkFreeMemory(*logicalDevice, indexBufferMemory, nullptr);

    vkDestroyBuffer(*logicalDevice, vertexBuffer, nullptr);
    vkFreeMemory(*logicalDevice, vertexBufferMemory, nullptr);
}

void Engine::Model::updateUniformBuffer(uint32_t currentImage) {
    std::cout << keyInput.keys.W.press << "\t" << (*mainProg)->keyInput.keys.W.press << std::endl;
    // Calculate time
    // Static can preserve the variable value (init once only), so calling the function next time will have the original value
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
    time = 0.0f;
    // On key press test
    if (keyInput.keys.W.press) {
        viewCoor[0] += 0.01f;
    }
    if (keyInput.keys.A.press) {
        viewCoor[1] += 0.01f;
    }
    if (keyInput.keys.S.press) {
        viewCoor[0] -= 0.01f;
    }
    if (keyInput.keys.D.press) {
        viewCoor[1] -= 0.01f;
    }
    if (keyInput.keys.G.press) {
        viewCoor[2] += 0.01f;
    }
    if (keyInput.keys.B.press) {
        viewCoor[2] -= 0.01f;
    }
    if (keyInput.keys.T.press) {
        for (int temp = 0; temp < 3; temp++)
            viewCoor[temp] = 2.0f;
    }
    if (keyInput.keys.zero.press) {
        for (int temp = 0; temp < 3; temp++)
            viewCoor[temp] = 0.0f;
    }
    //std::cout << x << " " << y << " " << z <<std::endl;
    // Define model, view and projection transformations
    UniformBufferObject ubo{};
    // Rotate Z with 90 degrees/s
    VkExtent2D swapChainExtent = *(*mainProg)->swapchain->getSwapChainExtent();
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(viewCoor[0], viewCoor[1], viewCoor[2]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(90.0f), swapChainExtent.width / (float)swapChainExtent.height, 0.1f, 10.0f);
    ubo.proj[1][1] *= -1; // The Y-coordinate is inverted in OpenGL

    void* data;
    vkMapMemory(*(*mainProg)->device->getLogicalDevice(), uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(*(*mainProg)->device->getLogicalDevice(), uniformBuffersMemory[currentImage]);
}