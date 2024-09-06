//
// Created by amenosora on 9/6/24.
//

#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "types.h"
#include <string>
namespace engine::vk
{

        class Window {
        public:

                Window() = delete;
                Window(i32 w, i32 h, const std::string& name);
                Window(const Window&) = delete;
                Window& operator=(const Window&) = delete;
                ~Window();
                [[nodiscard]] bool IsOpen() const;
                [[nodiscard]] VkExtent2D GetExtent() const { return { static_cast<ui32>(width), static_cast<ui32>(height) }; }
                [[nodiscard]] int WasWindowResized() const { return frameBufferResized; }
                void ResetWindowResizedFlag() { frameBufferResized = false; };
                void GetWinSurface(VkInstance inst, VkSurfaceKHR* surface) const;
                [[nodiscard]] GLFWwindow* GetGlfwWin() const { return window; };
        private:
                static void FrameBufferResizedCallBack(GLFWwindow* window, i32 w, i32 h);
                void InitWindow();
                const std::string windowName;
                i32 width;
                i32 height;
                GLFWwindow* window{};
                bool frameBufferResized = false;
        };
} //namespace engine
