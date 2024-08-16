#include "Triangle.h"
#include <stdexcept>
#include <array>
namespace engine
{
	void Triangle::run()
	{
		while (Window.IsOpen())
		{
			glfwPollEvents();
			DrawFrame();
		}
		vkDeviceWaitIdle(device.GetDevice());
	}

	void Triangle::LoadVertexParser()
	{
		const std::vector<Vertex> vertices = {
			{ {-1.0f, 1.0f}, { 1.0f, 1.0f, 0.0f }},
			{ {0.0f, 1.0f},  { 1.0f, 1.0f, 0.0f }},
			{ {-0.5f, 0.0f}, { 1.0f, 1.0f, 0.0f }},
			{{0.0f, 1.0f}, { 1.0f, 1.0f, 0.0f }},
			{{0.5f, 1.0f}, { 1.0f, 1.0f, 0.0f }},
			{{1.0f, 1.0f}, { 1.0f, 1.0f, 0.0f }}
	};
		vertexParser = std::make_unique<VkVertexParser>(device, vertices);
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
		PipeLineConfigInfo pipelineConfig = {};
		Pipeline::DefaultPipeLineConfigInfo(pipelineConfig, swapChain->Width(), swapChain->Height());
		pipelineConfig.renderPass = swapChain->GetRenderPass();
		pipelineConfig.pipelineLayout = pipelineLayout;
		Vkpipeline = std::make_unique<Pipeline>(device, "shaders/vert.spv", "shaders/frag.spv", pipelineConfig);
	}

	void Triangle::CreateCmdBuffers()
	{
		commandBuffers.resize(swapChain->GetImageCount());
		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = device.GetCommandPool();
		allocInfo.commandBufferCount = static_cast<ui32>(commandBuffers.size());

		if (vkAllocateCommandBuffers(device.GetDevice(), &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("Cannot allocate command buffers");
		}


	}

	void Triangle::DrawFrame()
	{
		ui32 imgIdx;
		VkResult result = swapChain->AcquireNextImage(&imgIdx);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain images");
		}
		RecordCommandBuffer(imgIdx);
		result = swapChain->SubmitCommandBuffers(&commandBuffers[imgIdx], &imgIdx);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || Window.WasWindowResized())
		{
			Window.ResetWindowResizedFlag();
			RecreateSwapChain();
			return;
		}
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}
	}

	void Triangle::RecreateSwapChain()
	{
		auto extent = Window.GetExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = Window.GetExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(device.GetDevice());
		swapChain = nullptr;
		swapChain = std::make_unique<VkSwapChain>(device, extent);
		CreatePipeline();
	}

	void Triangle::RecordCommandBuffer(i32 imgIdx)
	{
		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(commandBuffers[imgIdx], &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}
		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain->GetRenderPass();
		renderPassInfo.framebuffer = swapChain->GetFrameBuffer(imgIdx);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = swapChain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<ui32>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(commandBuffers[imgIdx], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		Vkpipeline->BindPipeline(commandBuffers[imgIdx]);
		vertexParser->Bind(commandBuffers[imgIdx]);
		vertexParser->Draw(commandBuffers[imgIdx]);
		vkCmdEndRenderPass(commandBuffers[imgIdx]);
		if (vkEndCommandBuffer(commandBuffers[imgIdx]) != VK_SUCCESS)
		{
			throw std::runtime_error("Couldn't bind command buffer");
		}
	}

	Triangle::Triangle()
	{
		LoadVertexParser();
		CreatePipelineLayout();
		RecreateSwapChain();
		CreateCmdBuffers();
	}

	Triangle::~Triangle()
	{
		vkDestroyPipelineLayout(device.GetDevice(), pipelineLayout, nullptr);
	}
}
//namespace engine