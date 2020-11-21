#include "pipeline.h"
#include "model.h"
#include <iostream>
#include <fstream>

Engine::Pipeline::Pipeline(mainProgram** mainProgramPtr)
{
    logicalDevice = (*mainProgramPtr)->device->getLogicalDevice();

    auto vertShaderCode = readFile("shaders/vert.spv");
    auto fragShaderCode = readFile("shaders/frag.spv");

    VkShaderModule vertShaderModule = createShaderModule(vertShaderCode);
    VkShaderModule fragShaderModule = createShaderModule(fragShaderCode);

    // Defines which shader stage the sahder is going to be used
    VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    // Defines the shader module to be used and the function to invoke
    vertShaderStageInfo.module = vertShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = fragShaderModule;
    fragShaderStageInfo.pName = "main";

    // For creating pipeline
    VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

    VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    // Setup this to let pipeline to accept the input from the format defined in C++ code
    auto bindingDescription = Engine::Model::Vertex::getBindingDescription();
    auto attributeDescriptions = Engine::Model::Vertex::getAttributeDescriptions();
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributeDescriptions.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
    vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

    // What kind of geometry will be drawn and if primitive restart should be enabled
    VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    // POINT_LIST:      Points from vertices
    // LINE_LIST:       Line from every 2 vertices without reuse
    // LINE_STRIP:      The end vertex of every line is used as start vertex for the next line
    // TRIANGLE_LIST:   Triangle from every 3 vertices without reuse
    // TRIANGLE_STRIP:  The second and third vertex of every triangle are used as first two vertices of the next triangle
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkExtent2D swapChainExtent = *(*mainProgramPtr)->swapchain->getSwapChainExtent();
    // The image location (?)
    VkViewport viewport = {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = (float)swapChainExtent.width;
    viewport.height = (float)swapChainExtent.height;
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;

    // Crop the images
    VkRect2D scissor = {};
    scissor.offset = { 0, 0 };
    scissor.extent = swapChainExtent;

    // Combine the viewport and the scissor
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &scissor;

    // Takes geometry that is shaped by vertices from vertex shader and turns it to fragments to be colored by fragment shader
    VkPipelineRasterizationStateCreateInfo rasterizer = {};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    // I don't know what is this
    rasterizer.depthClampEnable = VK_FALSE;
    // If true, geometry never passes through the rasterizer, disables any output to the frame buffer
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    // FILL:    Fill the area of the polygon with fragments
    // LINE:    Polygon edges are drawn as lines
    // POINT:   Polygon vertices are drawn as points
    rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
    // Line width
    rasterizer.lineWidth = 1.0f;
    // Type of face culling to use
    // For fixing nothing show
    rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    // For shadow mapping, unused for now
    rasterizer.depthBiasEnable = VK_FALSE;
    // For multisampling
    VkPipelineMultisampleStateCreateInfo multisampling = {};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_TRUE;
    multisampling.minSampleShading = .2f; // closer to 1 is smoother
    multisampling.rasterizationSamples = *(*mainProgramPtr)->device->getMSAASamples(false);

    // Depth stencil
    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = VK_TRUE; // if the depth of new fragments should be compared to the depth buffer to see if they should be discarded
    depthStencil.depthWriteEnable = VK_TRUE; // if the new depth of fragments that pass the depth test should actually be written to the depth buffer
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS; // The comparison that is performed to keep or discard fragments
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    // Combine with color
    // Attachment contains the configuration per attached framebuffer
    VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
    // Combine the colors with bitwise operator
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    // VK_FALSE means the new color from the fragment shader is passed through unmodified
    colorBlendAttachment.blendEnable = VK_FALSE;

    // Global color blending settings, for all framebuffers
    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    // If true, it will use the second method (bitwise combination), disables blendEnable in colorBlendAttachment
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f; // Optional
    colorBlending.blendConstants[1] = 0.0f; // Optional
    colorBlending.blendConstants[2] = 0.0f; // Optional
    colorBlending.blendConstants[3] = 0.0f; // Optional

    // Bunch of craps that I don't understand, what uniform values?
    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = (*mainProgramPtr)->descriptorSet->getDescriptorSetLayout();

    if (vkCreatePipelineLayout(*logicalDevice, &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create pipeline layout!");
    }

    // Create pipeline
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = 2;
    pipelineInfo.pStages = shaderStages;

    // Reference all info
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = nullptr; // Optional
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.renderPass = *(*mainProgramPtr)->renderPass->getRenderPass();
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional

    // Create graphics pipeline
    if (vkCreateGraphicsPipelines(*logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &graphicsPipeline) != VK_SUCCESS) {
        throw std::runtime_error("failed to create graphics pipeline!");
    }

    // Destroy the shader module upon the graphics pipeline is created
    vkDestroyShaderModule(*logicalDevice, fragShaderModule, nullptr);
    vkDestroyShaderModule(*logicalDevice, vertShaderModule, nullptr);
}

std::vector<char> Engine::Pipeline::readFile(const std::string& filename) {
    // ate means start reading at the end of the file
    // binary means read the file as binary file (avoid text transformations)
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    // If the file is not opened, throw an exception
    if (!file.is_open())
        throw std::runtime_error("failed to open file!");

    // tellg returns the current position, which the position is at the end, this can calculate the size of the file
    size_t fileSize = (size_t)file.tellg();
    std::vector<char> buffer(fileSize);

    // Read from start
    file.seekg(0);
    file.read(buffer.data(), fileSize);
    file.close();

    return buffer;
}

VkShaderModule Engine::Pipeline::createShaderModule(const std::vector<char>& code) {
    // Only create a pointer to the buffer with the bytecode and length
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());
    // Create shader module
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(*logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("failed to create shader module!");
    }
    return shaderModule;
}