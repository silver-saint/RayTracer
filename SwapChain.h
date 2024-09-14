#pragma once
#include "Device.h"
#include <vulkan/vulkan.h>
namespace vk::engine
{
	class SwapChain
	{
	public:
		SwapChain() = delete;
		SwapChain(Device& deviceRef);
		~SwapChain();
		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;
		[[nodiscard]] ui32 GetWidth() const { return swapChainExtent.width; }
		[[nodiscard]] ui32 GetHeight() const { return swapChainExtent.height; }
		[[nodiscard]] VkExtent2D GetSwapChainExtent() const { return swapChainExtent; }
		[[nodiscard]] VkFormat GetSwapChainImageFormat() const { return swapChainImageFormat; }
		[[nodiscard]] f32 GetExtentAspectRatio() const { return static_cast<f32>(swapChainExtent.width) / static_cast<f32>(swapChainExtent.height); }
		[[nodiscard]] VkRenderPass GetRenderPass() const { return renderPass; }
		[[nodiscard]] VkFramebuffer GetCurrentFrameBuffer(size_t idx) { return swapChainFramebuffers[idx]; }
	private:
		//init
		void Init();
		//SwapChain
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateFrameBuffers();
		Device& device;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		VkRenderPass renderPass;
	};
}
