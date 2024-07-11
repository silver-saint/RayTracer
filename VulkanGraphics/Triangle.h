//
// Created by rinrin on 7/10/24.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <GLFW/glfw3.h>
#include <vulkan/vulkan_core.h>
#include <vector>
#include <array>
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
    void InitVulkan();
    void CheckExtensions();
    void MainLoop();
    void Cleanup();
//member variables
    GLFWwindow* window;
    VkInstance instance;
    const char* NAME = "RayTracer";
    const int32_t WIDTH = 800;
    const int32_t HEIGHT = 600;
};



#endif //TRIANGLE_H
