#include "Device.h"
#include <stdexcept>
#include <vector>
#include "types.h"

engine::vk::Device::Device(Window& windowRef)
    : win(windowRef)
{
    Init();
}

engine::vk::Device::~Device()
{
    vkDestroyInstance(instance, nullptr);
}

void engine::vk::Device::Init()
{
    CreateInstance();
}

void engine::vk::Device::CreateInstance()
{
    VkApplicationInfo appInfo{};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "RayTracer";
    appInfo.pEngineName = "none";
    appInfo.engineVersion = VK_API_VERSION_1_3;
    appInfo.apiVersion = VK_API_VERSION_1_3;

    ui32 glfwExtensionsCount = 0;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    std::vector<const char*> requiredExtensions;

    for (uint32_t i = 0; i < glfwExtensionsCount; i++) {

        requiredExtensions.emplace_back(glfwExtensions[i]);
    }

    requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    VkInstanceCreateInfo instanceInfo{};
    instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instanceInfo.pNext = nullptr;
    instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
    instanceInfo.enabledExtensionCount = static_cast<ui32>(requiredExtensions.size());
    instanceInfo.enabledLayerCount = 0;
    instanceInfo.pApplicationInfo = &appInfo;
    instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();

    if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS)
    {
        throw std::runtime_error("Cannot create Instance");
    }





}