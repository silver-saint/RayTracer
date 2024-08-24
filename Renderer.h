#pragma once
#include <memory>
#include <vector>
#include "VkWindow.h"
#include "VkDeviceCtx.h"
#include "VkSwapChain.h"
#include <cassert>
namespace engine
{
	class Renderer
	{
	public:
		Renderer(VkWindow& window, VkDeviceCtx& deviceCtx);
		~Renderer();
		Renderer& operator=(const Renderer&) = delete;
		Renderer(const Renderer&) = delete;

		VkRenderPass GetSwapChainRenderPass() const { return swapChain->GetRenderPass(); };
		VkCommandBuffer BeginFrame();
		void EndFrame();
		bool IsFrameInProgress() const { return isFrameStarted; }
		VkCommandBuffer GetCurrentCmdBuffer() const
		{
			assert(isFrameStarted && "Cannot get commandBuffer when frame is not in progress");
			return commandBuffers[currFrameIdx];
		}
		void BeginSwapChainRenderPass(VkCommandBuffer cmdbuff);
		void EndSwapChainRenderPass(VkCommandBuffer cmdbuff);
		i32 GetFrameIndex() const
		{
			assert(isFrameStarted && "Cannot get frame index when frame not in progress");
			return currFrameIdx;
		}

	private:
		   void CreateCmdBuffers();
		   void FreeCmdBuffers();
		   void RecreateSwapChain();

		   VkWindow& window;
		   VkDeviceCtx& device;
		   std::unique_ptr<VkSwapChain> swapChain;
		   std::vector<VkCommandBuffer> commandBuffers;
		   ui32 currImgIdx;
		   i32 currFrameIdx = 0;
		   bool isFrameStarted = false;
	};
}
