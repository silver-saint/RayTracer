#pragma once
#include <string>
#include <vector>
#include "VkDeviceCtx.h"
namespace engine
{
	struct PipeLineConfigInfo 
	{
		VkViewport viewport;
		VkRect2D scissor;
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo raserizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};
	class Pipeline
	{
	public:
		Pipeline(VkDeviceCtx &deviceRef,
				 const std::string& vertexFP, 
				 const std::string& fragmentFP,
				 const PipeLineConfigInfo& configInfo);
		~Pipeline() {};
		Pipeline(const Pipeline&) = delete;
		void operator=(const Pipeline&) = delete;
		static PipeLineConfigInfo DefaultPipeLineConfigInfo(ui32 w, ui32 h);

	private:
		static std::vector<char> readFile(const std::string& fp);
		void CreateGraphicsPipeLine(const std::string& vertexFP, const std::string& fragmentFP, const PipeLineConfigInfo& configInfo);
		void CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);
		VkDeviceCtx& device;
		VkPipeline graphicsPipeline;
		VkShaderModule vertexShaderModule;
		VkShaderModule fragmentShaderModule;
	};
}
