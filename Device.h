//
// Created by amenosora on 9/6/24.
//

#pragma once
#include "Window.h"
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

namespace  vk::engine {

#ifdef NDEBUG
    const bool VALIDATIONLAYERS = false;
#else
    const bool VALIDATIONLAYERS = true;
#endif
    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        [[nodiscard]] bool isComplete() const { return graphicsFamily.has_value(); };
    };

    class Device
    {
    public:
        explicit Device(Window& windowRef);
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
        ui32 RateDeviceSuitability(VkPhysicalDevice device);
        //queues
        QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
        //validation layer functions;
        void SetupDebugMessenger();
        bool CheckValidationLayerSupport();
        void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
        std::vector<const char*> GetRequiredExtensions();

        //member variables
        VkInstance instance{};
        Window& win;

        VkDebugUtilsMessengerEXT debugMessenger{};
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkQueue graphicsQueue{};
        VkDevice device{};

        const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    };
}