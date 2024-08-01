#pragma once
#include "VkWindow.h"
#include <vector>
#include <optional>
#include <set>
#include <unordered_set>
namespace engine
{

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;
		bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};


	struct SwapChainSupportDetails
	{
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

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
		void SetupDebugMessenger();
		void CreateSurface();
		void PickPhysicalDevice();
		void CreateLogicalDevice();
		// void CreateCommandPool();



		//helper functions
		bool IsDeviceSuitable(VkPhysicalDevice device);
		std::vector<const char*> GetRequiredExtensions();
		bool CheckValidationLayerSupport();
		bool CheckExtensionSupport(VkPhysicalDevice device);
		QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		void HasRequiredExtensions();
		void PopulateDebugInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
		SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
		
		
		VkInstance instance;
		VkDebugUtilsMessengerEXT debugMessenger;
		VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
		VkWindow& win;
		VkCommandPool commandPool;


		VkDevice device;
		VkSurfaceKHR surface;
		VkQueue graphicsQueue;
		VkQueue presentQueue;
		
		const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
		const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	};

}
