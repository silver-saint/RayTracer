#include "Renderer.h"
#include <stdexcept>
vk::engine::Renderer::Renderer(Window& window, Device& deviceRef)
	: win {window}, device {deviceRef}
{
	Init();
}

void vk::engine::Renderer::Init()
{
	CreateCommandBuffer();

}

void vk::engine::Renderer::CreateCommandBuffer()
{
	VkCommandBufferAllocateInfo allocInfo = {};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = device.GetCommandPool();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = 1;

	if (vkAllocateCommandBuffers(device.GetDevice(), &allocInfo, &commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
}

void vk::engine::Renderer::DrawFrame()
{
	swapChain->AcquireNextImage(&imageIdx);
	vkResetCommandBuffer(commandBuffer, 0);
	BeginRenderPass(commandBuffer, imageIdx);
	swapChain->SubmitToCommandBuffer(&commandBuffer, &imageIdx);
	
}

void vk::engine::Renderer::BeginRenderPass(VkCommandBuffer buffer, ui32 imgIdx)
{
	VkCommandBufferBeginInfo beginInfo = {};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to write to command buffer!");
	}
	VkRenderPassBeginInfo renderPassInfo = {};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = swapChain->GetRenderPass();
	renderPassInfo.framebuffer = swapChain->GetCurrentFrameBuffer(imgIdx);
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapChain->GetSwapChainExtent();
	VkClearValue clearColor = { {{0.0f, 0.0f, 0.0f, 1.0f}} };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	pipeline->bindCommandBuffer(commandBuffer);
	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<f32>(swapChain->GetWidth());
	viewport.height = static_cast<f32>(swapChain->GetHeight());
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

	VkRect2D scissor = {};
	scissor.offset = { 0, 0 };
	scissor.extent = swapChain->GetSwapChainExtent();
	vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

	vkCmdDraw(commandBuffer, 3, 1, 0, 0);

}

void vk::engine::Renderer::EndRenderPass(VkCommandBuffer buffer)
{
	vkCmdEndRenderPass(commandBuffer);

	if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to end renderpass!");
	}
}
