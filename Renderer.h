#pragma once
#include "Device.h"
#include "SwapChain.h"
#include "Window.h"
#include "Pipeline.h"
#include <memory>
namespace vk::engine
{
	class Renderer
	{
	public:
		Renderer(Window& win, Device& deviceRef);
	private:
		void Init();
		void CreateCommandBuffer();
		void BeginRenderPass(VkCommandBuffer buffer, ui32 imgIdx);
		void EndRenderPass(VkCommandBuffer buffer);
		VkCommandBuffer commandBuffer;
		Device& device;
		Window& win;
		std::unique_ptr<Pipeline> pipeline;
		std::unique_ptr<SwapChain> swapChain;
		ui32 imageIdx = { 0 };
	};

}