#pragma once
#include <vulkan/vulkan.h>
#include "types.h"
#include <vector>
#include "Device.h"
namespace vk::engine
{
	struct PipelineConfigInfo
	{
		PipelineConfigInfo() = default;
		PipelineConfigInfo(const PipelineConfigInfo&) = delete;
		PipelineConfigInfo& operator=(const PipelineConfigInfo&) = delete;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicState;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};
	class Pipeline
	{
	public:

		Pipeline(Device& deviceRef,const std::string& vertFP, const std::string& fragFP, const PipelineConfigInfo& pipelineConfig);
		~Pipeline();
		void bindCommandBuffer(VkCommandBuffer buffer);
	private:
		void CreateGraphicsPipeline(const std::string& vertFP, const std::string& fragFP, const PipelineConfigInfo& pipelineConfig);
		VkShaderModule CreateShaderModule(const std::vector<char>& code);
		void SetPipelineConfig(PipelineConfigInfo& pipeline);
		Device& device;
		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;
		VkPipelineLayout pipelineLayout;

	};
}
