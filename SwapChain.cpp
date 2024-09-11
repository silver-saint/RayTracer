#include "SwapChain.h"
#include <array>
namespace vk::engine
{
	SwapChain::SwapChain(Device& deviceRef)
		: device{ deviceRef }
	{
		Init();
	}

	SwapChain::~SwapChain()
	{
		vkDestroySwapchainKHR(device.GetDevice(), swapChain, nullptr);
	}

	void SwapChain::Init()
	{
		CreateSwapChain();
	}

	void SwapChain::CreateSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = device.QuerySwapChainSupport(device.GetPhysicalDevice());

		VkSurfaceFormatKHR surfaceFormat = device.ChooseSwapSurfaceFormat(swapChainSupport.formats);
		VkPresentModeKHR presentMode = device.ChooseSwapPresentMode(swapChainSupport.presentModes);
		VkExtent2D extent = device.ChooseSwapExtent(swapChainSupport.capabilities);
		ui32 imageCount = swapChainSupport.capabilities.minImageCount;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
			imageCount = swapChainSupport.capabilities.maxImageCount;
		}
		VkSwapchainCreateInfoKHR swapChainInfo = {};
		swapChainInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapChainInfo.surface = device.GetSurface();
		swapChainInfo.pNext = nullptr;
		swapChainInfo.flags = 0;
		swapChainInfo.imageArrayLayers = 1;
		swapChainInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapChainInfo.minImageCount = imageCount;
		swapChainInfo.clipped = VK_TRUE;
		swapChainInfo.imageFormat = surfaceFormat.format;
		swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		swapChainInfo.queueFamilyIndexCount = 0;
		swapChainInfo.pQueueFamilyIndices = VK_NULL_HANDLE;
		swapChainInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapChainInfo.imageExtent = extent;
		swapChainInfo.presentMode = presentMode;
		swapChainInfo.preTransform = swapChainSupport.capabilities.currentTransform;
		swapChainInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapChainInfo.oldSwapchain = VK_NULL_HANDLE;

		QueueFamilyIndices indices = device.FindQueueFamilies(device.GetPhysicalDevice());
		std::array<ui32, 2> queueFamilyIndices = { indices.graphicsFamily.value(), indices.presentFamily.value() };

		if (indices.graphicsFamily != indices.presentFamily) {
			swapChainInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapChainInfo.queueFamilyIndexCount = 2;
			swapChainInfo.pQueueFamilyIndices = queueFamilyIndices.data();
		}
		else {
			swapChainInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
			swapChainInfo.queueFamilyIndexCount = 0; // Optional
			swapChainInfo.pQueueFamilyIndices = nullptr; // Optional
		}


		if (vkCreateSwapchainKHR(device.GetDevice(), &swapChainInfo, nullptr, &swapChain) != VK_SUCCESS) {
			throw std::runtime_error("failed to create swap chain!");
		}
		/*
		vkGetSwapchainImagesKHR(device.GetDevice(), swapChain, &imageCount, nullptr);
		swapChainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(device.GetDevice(), swapChain, &imageCount, swapChainImages.data());

		swapChainImageFormat = surfaceFormat.format;
		swapChainExtent = extent;
		*/
	}
}