#pragma once
#include "VkDeviceCtx.h"
#include <memory>
#include <array>
#include <string>
namespace engine

{
	class VkSwapChain
	{
	public:
		static constexpr i32 MAX_FRAMES_IN_FLIGHT = 2;
		VkSwapChain(VkDeviceCtx& deviceRef, VkExtent2D extent);
		VkSwapChain(VkDeviceCtx& deviceRef, VkExtent2D extent, std::shared_ptr<VkSwapChain> previous);
		~VkSwapChain();
		VkSwapChain(const VkSwapChain&) = delete;
		VkSwapChain& operator=(const VkSwapChain&) = delete;
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
		bool CompareSwapFormats(const VkSwapChain& swapChain) const {
			return swapChain.swapChainDepthFormat == swapChainDepthFormat &&
				swapChain.swapChainImageFormat == swapChainImageFormat;
		}

	private:
		void Init();
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
		VkFormat swapChainDepthFormat;
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
		std::shared_ptr<VkSwapChain> oldSwapChain;


		std::vector<VkSemaphore> imageAvailableSemaphores;
		std::vector<VkSemaphore> renderFinishedSemaphores;
		std::vector<VkFence> inFlightFences;
		std::vector<VkFence> imagesInFlight;
		size_t currentFrame = 0;
	};

} //namespace engine