#pragma once
#include "VkDeviceCtx.h"
#include <memory>
#include <array>
#include <string>
namespace engine

{
	class SwapChain
	{
	public:
		static constexpr i32 MAX_FRAMES_IN_FLIGHT = 2;
		SwapChain(VkDeviceCtx& deviceRef, VkExtent2D extent);
		~SwapChain();
		SwapChain(const SwapChain&) = delete;
		SwapChain& operator=(const SwapChain&) = delete;
		VkFramebuffer GetFrameBuffer(size_t idx) const { return swapChainFramebuffers[idx]; }
		VkRenderPass GetRenderPass() { return renderPass; }
		VkImageView GetImageView(size_t idx) const { return swapChainImageViews[idx]; }
		size_t GetImageCount() const { return swapChainImages.size(); }
		VkFormat GetSwapChainImageFormat() { return swapChainImageFormat; }
		VkExtent2D GetSwapChainExtent() { return swapChainExtent; }
		ui32 Width() const { return swapChainExtent.width; }
		ui32 Height() const { return swapChainExtent.height; }
		float ExtentAspectRatio() 
		{ 
			return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height); 
		}
		VkFormat FindDepthFormat();

		VkResult AcquireNextImage(uint32_t* imgIdx);
		VkResult SubmitCommandBuffers(const VkCommandBuffer* buffers, uint32_t* imgIdx);


	private:
		void CreateSwapChain();
		void CreateImageViews();
		void CreateDepthResources();
		void CreateRenderPass();
		void CreateFramebuffers();
		void CreateSyncObjects();

		VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
		VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
		VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;

		std::vector<VkFramebuffer> swapChainFramebuffers;
		VkRenderPass renderPass;

		std::vector<VkImage> depthImages;
		std::vector<VkDeviceMemory> depthImageMemorys;
		std::vector<VkImageView> depthImageViews;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;

		VkDeviceCtx& device;
		VkExtent2D windowExtent;

		VkSwapchainKHR swapChain;

		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame = 0;
	};

}