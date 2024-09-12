#pragma once
#include "Device.h"

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

	private:
		//init
		void Init();
		//SwapChain
		void CreateSwapChain();
		void CreateImageViews();
		Device& device;
		VkSwapchainKHR swapChain;
		std::vector<VkImage> swapChainImages;
		std::vector<VkImageView> swapChainImageViews;
		VkFormat swapChainImageFormat;
		VkExtent2D swapChainExtent;
	};
}
