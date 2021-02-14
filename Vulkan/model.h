#ifndef MODELVERTEX
#define MODELVERTEX


#pragma once
// Include GLFW
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
// Depth buffer
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
// Include GLM for vertex buffers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/hash.hpp>

#include <array>
#include "main.h"
#include "keyInput.h"
namespace Engine {
    class mainProgram;
    class KeyInput;
    class Model {
    public:
        struct UniformBufferObject {
            glm::mat4 model;
            glm::mat4 view;
            glm::mat4 proj;
        };
        struct Vertex {
            glm::vec3 pos;
            glm::vec3 color;
            glm::vec2 texCoord;

            // Tell Vulkan how to pass the data to vertex shader after uploading to GPU memory
            static VkVertexInputBindingDescription getBindingDescription() {
                VkVertexInputBindingDescription bindingDescription{};
                bindingDescription.binding = 0; // Index of the binding in the array of bindings
                bindingDescription.stride = sizeof(Vertex);  // Number of bytes from one entry to the next

                //VK_VERTEX_INPUT_RATE_VERTEX:      Move to the next data entry after each vertex
                //VK_VERTEX_INPUT_RATE_INSTANCE:    Move to the next data entry after each instance

                bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

                return bindingDescription;
            }


            // Extract vertex data
            static std::array<VkVertexInputAttributeDescription, 3> getAttributeDescriptions() {
                std::array<VkVertexInputAttributeDescription, 3> attributeDescriptions{};
                attributeDescriptions[0].binding = 0; // Which binding do the vertex come from
                attributeDescriptions[0].location = 0; // Defined in vertex shader input

                // float  : VK_FORMAT_R32_SFLOAT
                // vec2   : VK_FORMAT_R32G32_SFLOAT
                // vec3   : VK_FORMAT_R32G32B32_SFLOAT
                // vec4   : VK_FORMAT_R32G32B32A32_SFLOAT
                // ivec2  : VK_FORMAT_R32G32_SINT, a 2-component vector of 32-bit signed integers
                // uvec4  : VK_FORMAT_R32G32B32A32_UINT, a 4-component vector of 32-bit unsigned integers
                // double : VK_FORMAT_R64_SFLOAT, a double-precision(64-bit) float

                attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
                attributeDescriptions[0].offset = offsetof(Vertex, pos);

                attributeDescriptions[1].binding = 0;
                attributeDescriptions[1].location = 1;
                attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
                // As format defines the byte size of attribute data and offset specifies the number of bytes since the start of the per-vertex data to read from, the offset is calculated from the start of this struct
                attributeDescriptions[1].offset = offsetof(Vertex, color);

                attributeDescriptions[2].binding = 0;
                attributeDescriptions[2].location = 2;
                attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
                attributeDescriptions[2].offset = offsetof(Vertex, texCoord);

                // For now the shader.vert has 2 layout(location=x) in, so we create 2 attribute descriptions

                return attributeDescriptions;
            }

            // For using this struct as key in a hash
            bool operator==(const Vertex& other) const {
                return pos == other.pos && color == other.color && texCoord == other.texCoord;
            }
        };

        Model(mainProgram** mainProgramPtr,KeyInput::keyboardKeys& keysref);
        ~Model();
        void loadModel();
        bool hasStencilComponent(VkFormat format);
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void createVertexBuffer();
        void createIndexBuffer();
        void createUniformBuffers();

        std::vector<VkBuffer>* getUniformBuffers();
        std::vector<VkDeviceMemory>* getUniformBuffersMemory();
        VkBuffer* getIndexBuffers();
        VkDeviceMemory* getIndexBuffersMemory();
        VkBuffer* getVertexBuffers();
        VkDeviceMemory* getVertexBuffersMemory();
        void destroyUniformBuffer();
        std::string model_path = "obj/viking_room.obj";

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;

        void updateUniformBuffer(uint32_t currentImage);
    private:
        mainProgram** mainProg;

        VkDevice* logicalDevice;
        VkBuffer vertexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkBuffer indexBuffer;
        VkDeviceMemory indexBufferMemory;
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;

        KeyInput::keyboardKeys& keys;
        float viewCoor[3] = { 2.0f, 2.0f, 2.0f };
    };
}

namespace std {
    template<> struct hash<Engine::Model::Vertex> {
        size_t operator()(Engine::Model::Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.pos) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.texCoord) << 1);
        }
    };
}

#endif // !MODELVERTEX

