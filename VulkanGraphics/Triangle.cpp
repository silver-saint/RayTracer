//
// Created by rinrin on 7/10/24.
//

#include "Triangle.h"


void Triangle::InitWindow() {
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    window = glfwCreateWindow(WIDTH, HEIGHT, NAME, nullptr, nullptr);
}

void Triangle::CreateInstance() {
    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = NAME;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "No Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    uint32_t glfwExtensionCount = 0;
    const char **glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;
    createInfo.enabledLayerCount = 0;
    VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
    if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to create instance");
    }
    CheckExtensions();
}

void Triangle::InitVulkan() {
    CreateInstance();
}

void Triangle::CheckExtensions() {
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
    uint32_t reqCount = 0;
    const char **reqExtensions = glfwGetRequiredInstanceExtensions(&reqCount); //mandatory extensions


    if (*reqExtensions == nullptr) {
        throw std::runtime_error("failed to get required extensions");
    }
    std::array<std::string, 2> requiredExtensions;
    for (size_t i = 0; i < reqCount; i++) {
        requiredExtensions[i] = *reqExtensions++;
    }
    std::array<bool, 2> reqExtAv = {false, false};
    int idx = 0;
    std::cout << "List Available extensions" << std::endl;
    for (const auto &reqExt: requiredExtensions) {
        for (const auto &extension: extensions) {
            {
                std::cout << extension.extensionName << " " << reqExt.c_str() << '\n';
                if (extension.extensionName == reqExt) {
                    reqExtAv[idx++] = true;
                }
            }
        }
    }

}

void Triangle::MainLoop() {
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
    }
}

void Triangle::Cleanup() {
    vkDestroyInstance(instance, nullptr);
    glfwDestroyWindow(window);
    glfwTerminate();
}
