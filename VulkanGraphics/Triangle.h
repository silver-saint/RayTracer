//
// Created by rinrin on 7/10/24.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <iostream>

class Triangle {
public:
    void Run() {
        InitWindow();
        InitVulkan();
        MainLoop();
        Cleanup();
    }
private:
//member functions
    void InitWindow();
    void CreateInstance();
    void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
    static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
                                                    VkDebugUtilsMessageTypeFlagsEXT messageType,
                                                    const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
                                                    void *pUserData);
    void InitVulkan();
    bool CheckValidationLayerSupport();
    std::vector<const char*> GetRequiredExtensions();
    void MainLoop();
    void Cleanup();
//member variables
    GLFWwindow* window;
    VkInstance instance;
    VkDebugUtilsMessengerEXT debugMessenger;
    const char* NAME = "RayTracer";
    const int32_t WIDTH = 800;
    const int32_t HEIGHT = 600;
    const std::vector<const char*> validationLayers = {"VK_LAYER_KHRONOS_validation"};
};



#endif //TRIANGLE_H
