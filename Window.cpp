//
// Created by amenosora on 9/6/24.
//

#include "Window.h"
#include <stdexcept>


    namespace engine::vk {
        Window::Window(i32 w, i32 h, const std::string& name)
            : width(w), height(h), windowName(name)
        {

            InitWindow();
        }

        Window::~Window()
        {
            glfwDestroyWindow(window);
        }

        bool Window::IsOpen() const {
            return !glfwWindowShouldClose(window);
        }
        void Window::FrameBufferResizedCallBack(GLFWwindow* window, i32 w, i32 h)
        {
            auto win = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
            win->frameBufferResized = true;
            win->width = w;
            win->height = h;
        }

        void Window::GetWinSurface(VkInstance inst, VkSurfaceKHR* surface) const {
            if (glfwCreateWindowSurface(inst, window, nullptr, surface) != VK_SUCCESS)
            {
                throw std::runtime_error("failed to create window surface!");
            }
        }

        void Window::InitWindow()
        {

            if (!glfwInit())
            {
                throw std::runtime_error("Error, couldn't init GLFW");
            }

            glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
            glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
            window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
            if (!window)
            {
                throw std::runtime_error("Error, couldn't init Window.");
            }
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, FrameBufferResizedCallBack);
        }
    }
// engine