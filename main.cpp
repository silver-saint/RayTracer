#define GLFW_INCLUDE_VULKAN
#include "VulkanGraphics/Triangle.h"
#include <iostream>
#include <stdexcept>
#include <cstdlib>



int main() {
    Triangle app;
    try {
        app.Run();
    }
    catch(const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
