#include "Pipeline.h"
#include <fstream>
#include <array>
#include <stdexcept>
namespace vk::engine
{
    Pipeline::Pipeline(Device& deviceRef, const std::string& vertFP, const std::string& fragFP, const PipelineConfigInfo& pipelineConfig)
        : device{ deviceRef } 
    {
        CreateGraphicsPipeline(vertFP, fragFP, pipelineConfig);
    }

    Pipeline::~Pipeline()
    {
        vkDestroyShaderModule(device.GetDevice(), vertShaderModule, nullptr);
        vkDestroyShaderModule(device.GetDevice(), fragShaderModule, nullptr);
        vkDestroyPipeline(device.GetDevice(), graphicsPipeline, nullptr);
        vkDestroyPipelineLayout(device.GetDevice(), pipelineLayout, nullptr);
        
    }

    void Pipeline::bindCommandBuffer(VkCommandBuffer buffer)
    {
        vkCmdBindPipeline(buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
    }

    static std::vector<char> readFile(const std::string& filename)
    {
        std::ifstream file(filename, std::ios::ate | std::ios::binary);

        if (!file.is_open()) {
            throw std::runtime_error("failed to open file!");
        }
        size_t fileSize = (size_t)file.tellg();
        std::vector<char> buffer(fileSize);
        file.seekg(0);
        file.read(buffer.data(), fileSize);
        file.close();

        return buffer;
    }

    void Pipeline::CreateGraphicsPipeline(const std::string& vertFP, const std::string& fragFP, const PipelineConfigInfo& pipelineConfig)
    {
        auto vertShaderCode = readFile(vertFP);
        auto fragShaderCode = readFile(fragFP);

         vertShaderModule = CreateShaderModule(vertShaderCode);
         fragShaderModule = CreateShaderModule(fragShaderCode);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

        VkGraphicsPipelineCreateInfo graphicsPipelineInfo = {};
        graphicsPipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        graphicsPipelineInfo.stageCount = 2;
        graphicsPipelineInfo.pStages = shaderStages.data();
        graphicsPipelineInfo.pVertexInputState = &vertexInputInfo;
        graphicsPipelineInfo.pInputAssemblyState = &pipelineConfig.inputAssemblyInfo;
        graphicsPipelineInfo.pViewportState = &pipelineConfig.viewportInfo;
        graphicsPipelineInfo.pRasterizationState = &pipelineConfig.rasterizationInfo;
        graphicsPipelineInfo.pMultisampleState = &pipelineConfig.multisampleInfo;
        graphicsPipelineInfo.pColorBlendState = &pipelineConfig.colorBlendInfo;
        graphicsPipelineInfo.pDepthStencilState = &pipelineConfig.depthStencilInfo;
        graphicsPipelineInfo.pDynamicState = &pipelineConfig.dynamicStateInfo;
        graphicsPipelineInfo.layout = pipelineLayout;
        graphicsPipelineInfo.renderPass = pipelineConfig.renderPass;
        graphicsPipelineInfo.subpass = pipelineConfig.subpass;
        graphicsPipelineInfo.basePipelineIndex = -1;
        graphicsPipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

        if (vkCreateGraphicsPipelines(
            device.GetDevice(),
            VK_NULL_HANDLE,
            1,
            &graphicsPipelineInfo,
            nullptr,
            &graphicsPipeline) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create graphics pipeline");
        }
    }

    VkShaderModule Pipeline::CreateShaderModule(const std::vector<char>& code)
    {
        VkShaderModuleCreateInfo moduleInfo = {};
        moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        moduleInfo.codeSize = code.size();
        moduleInfo.pCode = reinterpret_cast<const ui32*>(code.data());
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device.GetDevice(), &moduleInfo, nullptr, &shaderModule) != VK_SUCCESS) {
            throw std::runtime_error("failed to create shader module!");
        }
        return shaderModule;
    }
    void Pipeline::SetPipelineConfig(PipelineConfigInfo& pipeline)
    {
        
        pipeline.inputAssemblyInfo = {};
        pipeline.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        pipeline.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        pipeline.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;

        pipeline.viewportInfo = {};
        pipeline.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        pipeline.viewportInfo.viewportCount = 1;
        pipeline.viewportInfo.pViewports = nullptr;
        pipeline.viewportInfo.scissorCount = 1;
        pipeline.viewportInfo.pScissors = nullptr;
        pipeline.rasterizationInfo = {};
        pipeline.rasterizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        pipeline.rasterizationInfo.depthClampEnable = VK_FALSE;
        pipeline.rasterizationInfo.rasterizerDiscardEnable = VK_FALSE;
        pipeline.rasterizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
        pipeline.rasterizationInfo.lineWidth = 1.0f;
        pipeline.rasterizationInfo.cullMode = VK_CULL_MODE_NONE;
        pipeline.rasterizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
        pipeline.rasterizationInfo.depthBiasEnable = VK_FALSE;
        pipeline.rasterizationInfo.depthBiasConstantFactor = 0.0f;  // Optional
        pipeline.rasterizationInfo.depthBiasClamp = 0.0f;           // Optional
        pipeline.rasterizationInfo.depthBiasSlopeFactor = 0.0f;     // Optional
        
        pipeline.multisampleInfo = {};
        pipeline.multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        pipeline.multisampleInfo.sampleShadingEnable = VK_FALSE;
        pipeline.multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
        pipeline.multisampleInfo.minSampleShading = 1.0f;           // Optional
        pipeline.multisampleInfo.pSampleMask = nullptr;             // Optional
        pipeline.multisampleInfo.alphaToCoverageEnable = VK_FALSE;  // Optional
        pipeline.multisampleInfo.alphaToOneEnable = VK_FALSE;       // Optional
        
        
        pipeline.colorBlendAttachment = {};
        pipeline.colorBlendAttachment.colorWriteMask =
                   VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
                   VK_COLOR_COMPONENT_A_BIT;
        pipeline.colorBlendAttachment.blendEnable = VK_FALSE;
        pipeline.colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        pipeline.colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        pipeline.colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;              // Optional
        pipeline.colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;   // Optional
        pipeline.colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;  // Optional
        pipeline.colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;              // Optional
        
        pipeline.colorBlendInfo = {};
        pipeline.colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        pipeline.colorBlendInfo.logicOpEnable = VK_FALSE;
        pipeline.colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;  // Optional
        pipeline.colorBlendInfo.attachmentCount = 1;
        pipeline.colorBlendInfo.pAttachments = &pipeline.colorBlendAttachment;
        pipeline.colorBlendInfo.blendConstants[0] = 0.0f;  // Optional
        pipeline.colorBlendInfo.blendConstants[1] = 0.0f;  // Optional
        pipeline.colorBlendInfo.blendConstants[2] = 0.0f;  // Optional
        pipeline.colorBlendInfo.blendConstants[3] = 0.0f;  // Optional
        pipeline.depthStencilInfo = {};
        pipeline.depthStencilInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
        pipeline.depthStencilInfo.depthTestEnable = VK_TRUE;
        pipeline.depthStencilInfo.depthWriteEnable = VK_TRUE;
        pipeline.depthStencilInfo.depthCompareOp = VK_COMPARE_OP_LESS;
        pipeline.depthStencilInfo.depthBoundsTestEnable = VK_FALSE;
        pipeline.depthStencilInfo.minDepthBounds = 0.0f;  // Optional
        pipeline.depthStencilInfo.maxDepthBounds = 1.0f;  // Optional
        pipeline.depthStencilInfo.stencilTestEnable = VK_FALSE;
        pipeline.depthStencilInfo.front = {};  // Optional
        pipeline.depthStencilInfo.back = {};   // Optional

        pipeline.dynamicState = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };
        pipeline.dynamicStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        pipeline.dynamicStateInfo.pDynamicStates = pipeline.dynamicState.data();
        pipeline.dynamicStateInfo.dynamicStateCount =
            static_cast<uint32_t>(pipeline.dynamicState.size());
        pipeline.dynamicStateInfo.flags = 0;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0; // Optional
        pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
        pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
        pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

        if (vkCreatePipelineLayout(device.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }
} //namespace vk::engine;