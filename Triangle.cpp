#include "Triangle.h"
#include <stdexcept>
#include <array>
namespace engine
{
	void Triangle::run()
	{

		while (window.IsOpen())
		{
			glfwPollEvents();
			if (auto commandbuffer = renderer.BeginFrame())
			{
				renderer.BeginSwapChainRenderPass(commandbuffer);
				renderer.EndSwapChainRenderPass(commandbuffer);
				renderer.EndFrame();
			}
		}
		vkDeviceWaitIdle(device.GetDevice());
	}

	void Triangle::LoadVertexParser()
	{

		Builder builder;
		static Menu menu;
		menu.Init();
		switch (menu.GetInput())
		{
		case 1:
		{
			builder.vertices =
			{
			   {{-1.0f, 1.0f}, { 1.0f, 1.0f, 0.0f }},
			   {{-0.5f, 0.0f}, { 1.0f, 1.0f, 0.0f }},
			   {{0.0f, 1.0f},  { 1.0f, 1.0f, 0.0f }},
			   {{0.5f, 0.0f}, { 1.0f, 1.0f, 0.0f }},
			   {{1.0f, 1.0f}, { 1.0f, 1.0f, 0.0f }},
			   {{0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}}
			};
			builder.indicies = { 0,1,2,2,3,4,1,5,3 };
			break;
		}
		case 2:
		{
			builder.vertices =
			{
			   {{-0.5f, -0.5f}, { 1.0f, 1.0f, 0.0f }},
			   {{0.5f, -0.5f}, { 1.0f, 1.0f, 0.0f }},
			   {{0.5f, 0.5f},  { 1.0f, 1.0f, 0.0f }},
			   {{-0.5f, 0.5f}, { 1.0f, 1.0f, 0.0f }},
			};
			builder.indicies = { 0,1,2,2,3,0 };
			break;
		}
		default:
		{
			builder.vertices =
			{
			   {{0.0f, -0.5f}, { 1.0f, 0.0f, 0.0f }},
			   {{0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f }},
			   {{-0.5f, 0.5f},  { 0.0f, 0.0f, 1.0f }}
			};
			builder.indicies = { 0,1,2 };
			break;
		}
		}
		vertexParser = std::make_unique<VkVertexParser>(device, builder);
	}

	void Triangle::CreatePipelineLayout()
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

	void Triangle::CreatePipeline()
	{
		assert(pipelineLayout != nullptr && "Cannot create pipelinelayout before swapchain");
		PipeLineConfigInfo pipelineConfig = {};
		Pipeline::DefaultPipeLineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderer.GetSwapChainRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		Vkpipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
	}




	
	Triangle::Triangle()
	{
		LoadVertexParser();
		CreatePipelineLayout();
		CreatePipeline();
	}

	Triangle::~Triangle()
	{
		vkDestroyPipelineLayout(device.GetDevice(), pipelineLayout, nullptr);
	}
}
//namespace engine