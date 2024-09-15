#pragma once
#include "Device.h"
#include <vulkan/vulkan.h>
namespace vk::engine
{
	class SwapChain
	{
	public:
		static constexpr i32 MAX_FRAMES_IN_FLIGHT = 2;


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
		[[nodiscard]] VkFramebuffer GetCurrentFrameBuffer(size_t idx) const { return swapChainFramebuffers[idx]; }
		VkResult AcquireNextImage(ui32* imageIndex);
		VkResult SubmitToCommandBuffer(const VkCommandBuffer* buffers, ui32* imgIdx);
	private:
		//init
		void Init();
		//SwapChain
		void CreateSwapChain();
		void CreateImageViews();
		void CreateRenderPass();
		void CreateFrameBuffers();
		void CreateSyncObjects();
		Device& device;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
		VkRenderPass renderPass;


		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;
		size_t currentFrame = 0;
	};
}
