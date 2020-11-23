#include "model.h"
// tinyobjloader for 3D models
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

Engine::Model::Model(mainProgram** mainProgramPtr)
{
	mainProg = mainProgramPtr;
	loadModel();
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