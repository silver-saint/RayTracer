#pragma once
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
			bool isComplete() { return graphicsFamily.has_value(); };
		};

		class Device
		{
		public:
			Device(Window& windowRef);
			~Device();
			Device(const Device&) = delete;
			Device& operator=(const Device&) = delete;
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
			//queues
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
			//swapchain
			SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);
			//surface
			VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
			//extent;
			VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

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
			
			const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
			const std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

			VkSurfaceKHR surface;
		};
}
