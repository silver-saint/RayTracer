cmake_minimum_required(VERSION 3.29)
project(VulkanRayTracer)

set(CMAKE_CXX_STANDARD 20)
if(UNIX)
    message(STATUS "CREATING BUILD FOR UNIX")
endif()

add_executable(${PROJECT_NAME} RayTracer.cpp
        App.cpp
        Device.cpp
        Window.cpp)
find_package(Vulkan REQUIRED)
message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")
find_package(SDL2 REQUIRED)
find_package(glm REQUIRED)

include_directories({PROJECT_NAME} ${SDL2_INCLUDE_DIRS} ${VULKAN_INCLUDE_DIRS})
target_include_directories(${PROJECT_NAME} PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} glm::glm ${SDL2_LIBRARIES} Vulkan::Vulkan)
