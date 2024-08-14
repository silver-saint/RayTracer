#include "Triangle.h"

void engine::Triangle::run()
{
	while (Window.IsOpen())
	{
		glfwPollEvents();
	}
}

void engine::Triangle::CreatePipelineLayout()
{
	VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;

	if (vkCreatePipelineLayout(device.GetDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Pipeline Layout");
	}
}

void engine::Triangle::CreatePipeline()
{
	auto pipelineConfig = Pipeline::DefaultPipeLineConfigInfo(swapChain.Width(), swapChain.Height());
	pipelineConfig.renderPass = swapChain.GetRenderPass();
	pipelineConfig.pipelineLayout = pipelineLayout;
	Vkpipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
}

void engine::Triangle::CreateCmdBuffers()
{
}

void engine::Triangle::DrawFrame()
{
}

engine::Triangle::Triangle()
{
	CreatePipelineLayout();
	CreatePipeline();
	CreateCmdBuffers();
}

engine::Triangle::~Triangle() {}
