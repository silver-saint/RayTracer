#pragma once
#include <iostream>
#include "VkWindow.h"
#include <vector>

namespace engine
{
	class VkCtx
	{
	public:
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif
		VkCtx(VkWindow& window);
		~VkCtx();
		VkCtx(const VkCtx&) = delete;
		VkCtx& operator=(const VkCtx&) = delete;
		VkCtx(VkCtx&&) = delete;
		VkCtx& operator=(VkCtx&&) = delete;
	private:
		void CreateInstance();
		bool CheckValidationLayerSupport();
		std::vector<const char*> GetRequiredExtensions();

		//void PopulateDebugInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		VkWindow& win;
		VkInstance instance;
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};

}
