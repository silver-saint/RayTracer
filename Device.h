#pragma once
#include "types.h"
#include "Window.h"
#include <optional>
#include <vulkan/vulkan.h>
#include <vector>


namespace vk::engine
{

#ifdef NDEBUG
		const bool VALIDATIONLAYERS = false;
#else
		const bool VALIDATIONLAYERS = true;
#endif
		struct QueueFamilyIndices {
			std::optional<uint32_t> graphicsFamily;
			bool isComplete() { return graphicsFamily.has_value() };
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
			//instance
			void CreateInstance();
			//device
			void PickPhysicalDevice();
			bool isDeviceSuitable(VkPhysicalDevice device);
			i32 RateDeviceSuitability(VkPhysicalDevice device);
			//queues
			QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
			//validation layer functions;
			void SetupDebugMessenger();
			bool CheckValidationLayerSupport();
			void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
			std::vector<const char*> GetRequiredExtensions();
			VkInstance instance;
			VkDebugUtilsMessengerEXT debugMessenger;
			VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
			Window& win;
			const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
		};
}
