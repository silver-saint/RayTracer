#include "Device.h"

vk::engine::Device::Device(Window& windowRef)
	: win(windowRef)
{
	Init();
}

vk::engine::Device::~Device()
{
	vkDestroyInstance(instance, nullptr);
}

void vk::engine::Device::Init()
{
	CreateInstance();
}

void vk::engine::Device::CreateInstance()
{
	if (VALIDATIONLAYERS && !CheckValidationLayerSupport())
	{
		throw std::runtime_error("Validation layers could not be initialized");
	}
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "RayTracer";
	appInfo.pEngineName = "none";
	appInfo.engineVersion = VK_API_VERSION_1_3;
	appInfo.apiVersion = VK_API_VERSION_1_3;
	
	ui32 SDLExtensionsCount = 0;
	SDL_Vulkan_GetInstanceExtensions(win.GetWindow(), &SDLExtensionsCount, NULL);
	const char** SDLExtensions = new const char*[SDLExtensionsCount];
	SDL_Vulkan_GetInstanceExtensions(win.GetWindow(), &SDLExtensionsCount, SDLExtensions);

	std::vector<const char*> requiredExtensions;
	for (ui32 i = 0; i < SDLExtensionsCount; i++)
	{
		requiredExtensions.emplace_back(SDLExtensions[i]);
	}
	requiredExtensions.emplace_back(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

	VkInstanceCreateInfo instanceInfo{};
	instanceInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	instanceInfo.pNext = nullptr;
	instanceInfo.flags |= VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
	instanceInfo.enabledExtensionCount = static_cast<ui32>(requiredExtensions.size());
	if (VALIDATIONLAYERS)
	{
		instanceInfo.enabledLayerCount = static_cast<ui32>(validationLayers.size());
		instanceInfo.ppEnabledLayerNames = validationLayers.data();
	}
	else
	{
		instanceInfo.enabledLayerCount = 0;
	}
	instanceInfo.pApplicationInfo = &appInfo;
	instanceInfo.ppEnabledExtensionNames = requiredExtensions.data();
	
	if (vkCreateInstance(&instanceInfo, nullptr, &instance) != VK_SUCCESS)
	{
		throw std::runtime_error("Cannot create Instance");
	}

		



}

bool vk::engine::Device::CheckValidationLayerSupport()
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());
	bool layerFound = false;
	for (const char* layerName : validationLayers) 
	{
	for (const auto& layerProperties : availableLayers) {
		if (strcmp(layerName, layerProperties.layerName) == 0) {
			layerFound = true;
			break;
		}
	}

	if (!layerFound) {
		return false;
	}
}
	return true;
}
