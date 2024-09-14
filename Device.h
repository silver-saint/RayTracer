#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#include "types.h"
#include "Window.h"
#include <optional>
#include <vulkan/vulkan.h>
#include <vector>


namespace vk::engine
{
	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};
#ifdef NDEBUG
		const bool VALIDATIONLAYERS = false;
#else
		const bool VALIDATIONLAYERS = true;
#endif
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			std::optional<uint32_t> presentFamily;
			bool isComplete() { return graphicsFamily.has_value() && presentFamily.has_value(); };
		};

		class Device
		{
		public:
			Device(Window& windowRef);
			~Device();
			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;
			//swapchain
			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
			//Getters
			[[nodiscard]] VkPhysicalDevice GetPhysicalDevice() const { return physicalDevice; }
			[[nodiscard]] VkDevice GetDevice() const { return device; }
			[[nodiscard]] VkSurfaceKHR GetSurface() const { return surface; }
			[[nodiscard]] VkCommandPool GetCommandPool() const { return commandPool; }
			[[nodiscard]] VkQueue GetGraphicsQueue() const { return graphicsQueue; }
			[[nodiscard]] VkQueue GetPresentQueue() const { return presentQueue; }
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
			VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
			//queues
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
		private:
			void Init();
			// vulkan instance
			void CreateInstance();
			//device layer
			void PickPhysicalDevice();
			void CreateLogicalDevice();
			bool isDeviceSuitable(VkPhysicalDevice device);
			bool CheckDeviceExtension(VkPhysicalDevice device);
			i32 RateDeviceSuitability(VkPhysicalDevice device);
			//command pool
			void CreateCommandPool();
		

			//surface
			void CreateSurface();
			
			//validation layer functions;
			void SetupDebugMessenger();
			bool CheckValidationLayerSupport();
			void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
			std::vector<const char*> GetRequiredExtensions();
			
			//member variables
			VkInstance instance;
			Window& win;

			VkDebugUtilsMessengerEXT debugMessenger;
			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
			VkQueue graphicsQueue;
			VkDevice device;
			VkQueue presentQueue;
			const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
			const std::vector<const char*> deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

			VkSurfaceKHR surface;
			VkCommandPool commandPool;
		};
}
