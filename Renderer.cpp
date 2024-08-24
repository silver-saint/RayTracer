#include "Renderer.h"
#include <stdexcept>

namespace engine
{
	Renderer::Renderer(VkWindow& window, VkDeviceCtx& deviceCtx)
		: window{window}, device{deviceCtx}
	{
		RecreateSwapChain();
		CreateCmdBuffers();
	}

	Renderer::~Renderer()
	{
		FreeCmdBuffers();
	}

	VkCommandBuffer Renderer::BeginFrame()
	{
		assert(!isFrameStarted && "Can't call this function while in progress.");

		VkResult result = swapChain->AcquireNextImage(&currImgIdx);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			RecreateSwapChain();
			return nullptr;
		}
		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("Failed to acquire swap chain images");
		}

		isFrameStarted = true;

		auto cmdbuff = GetCurrentCmdBuffer();

		VkCommandBufferBeginInfo beginInfo = {};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		if (vkBeginCommandBuffer(cmdbuff, &beginInfo) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to begin recording command buffer!");
		}
		return cmdbuff;
	}

	void Renderer::EndFrame()
	{
		assert(isFrameStarted && "Can't call this function while is not in progress.");
		auto cmdbuff = GetCurrentCmdBuffer();
		if (vkEndCommandBuffer(cmdbuff) != VK_SUCCESS)
		{
			throw std::runtime_error("Couldn't record command buffer");
		}
		auto result = swapChain->SubmitCommandBuffers(&cmdbuff, &currImgIdx);
		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || window.WasWindowResized())
		{
			window.ResetWindowResizedFlag();
			RecreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}
		isFrameStarted = false;
		currFrameIdx = (currFrameIdx + 1) % VkSwapChain::MAX_FRAMES_IN_FLIGHT;
	}

	void Renderer::BeginSwapChainRenderPass(VkCommandBuffer cmdbuff)
	{
		assert(isFrameStarted && "Can't call BeginSwapChainRenderPass if frame is not in progress.");
		assert(cmdbuff == GetCurrentCmdBuffer() && "Can't begin render pass on the command buffer from a different frame");

		VkRenderPassBeginInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = swapChain->GetRenderPass();
		renderPassInfo.framebuffer = swapChain->GetFrameBuffer(currImgIdx);

		renderPassInfo.renderArea.offset = { 0,0 };
		renderPassInfo.renderArea.extent = swapChain->GetSwapChainExtent();

		std::array<VkClearValue, 2> clearValues = {};
		clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f };
		clearValues[1].depthStencil = { 1.0f, 0 };
		renderPassInfo.clearValueCount = static_cast<ui32>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();

		vkCmdBeginRenderPass(cmdbuff, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport = {};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		viewport.height = static_cast<f32>(swapChain->Height());
		viewport.width = static_cast<f32>(swapChain->Width());

		VkRect2D scissor = { {0,0}, swapChain->GetSwapChainExtent() };
		vkCmdSetViewport(cmdbuff, 0, 1, &viewport);
		vkCmdSetScissor(cmdbuff, 0, 1, &scissor);
	}

	void Renderer::EndSwapChainRenderPass(VkCommandBuffer cmdbuff)
	{
		assert(isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
		assert(
			cmdbuff == GetCurrentCmdBuffer() &&
			"Can't end render pass on command buffer from a different frame");
		vkCmdEndRenderPass(cmdbuff);
	}

	void Renderer::CreateCmdBuffers()
	{
		commandBuffers.resize(VkSwapChain::MAX_FRAMES_IN_FLIGHT);
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

	void Renderer::FreeCmdBuffers()
	{
		vkFreeCommandBuffers(device.GetDevice(), device.GetCommandPool(), static_cast<ui32>(commandBuffers.size()), commandBuffers.data());
		commandBuffers.clear();
	}

	

	void Renderer::RecreateSwapChain()
	{
		auto extent = window.GetExtent();
		while (extent.width == 0 || extent.height == 0)
		{
			extent = window.GetExtent();
			glfwWaitEvents();
		}
		vkDeviceWaitIdle(device.GetDevice());

		if (swapChain == nullptr)
		{
			swapChain = std::make_unique<VkSwapChain>(device, extent);
		}
		else
		{
		//	std::shared_ptr<VkSwapChain> oldSwapChain = std::move(swapChain);
			swapChain = std::make_unique<VkSwapChain>(device, extent);
			/*
			if (!oldSwapChain->CompareSwapFormats(*swapChain.get())) {
				throw std::runtime_error("Swap chain image(or depth) format has changed!");
			}
			*/

			if (swapChain->GetImageCount() != commandBuffers.size())
			{
				FreeCmdBuffers();
				CreateCmdBuffers();
			}
		}
		
	}
}
//namespace engine