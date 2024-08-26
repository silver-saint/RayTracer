#include "RenderSystem.h"
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <array>
#include <cassert>
#include <stdexcept>

namespace engine
{
	RenderSystem::RenderSystem(VkDeviceCtx& deviceCtxRef, VkRenderPass renderPass)
		:device{deviceCtxRef}
	{

		createPipelineLayout();
		createPipeline(renderPass);
	}
	RenderSystem::~RenderSystem()
	{
		vkDestroyPipelineLayout(device.GetDevice(), pipelineLayout, nullptr);
	}
	void RenderSystem::RenderObjects(VkCommandBuffer cmdbuffer, std::vector<GameObject>& gameObjects)
	{
		Vkpipeline->BindPipeline(cmdbuffer);

		for (auto& obj : gameObjects)
		{
			PushConstantData push {};
			push.offset = {0.0f, 0.0f};
			push.color = { 1.0f, 1.0f, 0.5f };
			vkCmdPushConstants(cmdbuffer, pipelineLayout, 
				VK_SHADER_STAGE_FRAGMENT_BIT | VK_SHADER_STAGE_VERTEX_BIT,
				0, 
				sizeof(PushConstantData), &push);
			obj.parser->Bind(cmdbuffer);
			obj.parser->Draw(cmdbuffer);

		}

	}
	void RenderSystem::createPipelineLayout()
	{
		VkPushConstantRange pushConstantRange {};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(PushConstantData);


		VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(device.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create Pipeline Layout");
		}
	}
	void RenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipelinelayout before swapchain");
		PipeLineConfigInfo pipelineConfig = {};
		Pipeline::DefaultPipeLineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		Vkpipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
	}
}
