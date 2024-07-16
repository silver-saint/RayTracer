#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>
#include "stdlibrary.h"
class VKApp
{
public:
    void run() {
        InitWindow();
        InitVulkan();
        MainLoop();
        CleanUp();
    }

private:
    //member functions
    void InitWindow();

    void InitVulkan();

    void MainLoop();
    void PickPhysicalDevice();
    void CleanUp();
    void CreateInstance();
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    void SetupDebugMessenger();
    void CreateLogicalDevice();
    bool CheckValidationLayerSupport();
    //member variables
    GLFWwindow* window;
    const char* NAME = "RayTracer";
    const uint32_t WIDTH = 800;
    const uint32_t HEIGHT = 600;
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;
};

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;

    bool isComplete() {
        return graphicsFamily.has_value();
    }
};
