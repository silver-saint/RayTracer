cmake_minimum_required(VERSION 3.28)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)
find_package(Vulkan REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
add_executable(${PROJECT_NAME} main.cpp
        VulkanGraphics/Triangle.cpp
        VulkanGraphics/Triangle.h)

target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES} Vulkan::Vulkan glm::glm)