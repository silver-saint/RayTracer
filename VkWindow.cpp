#include "vkWindow.h"

namespace engine
{
	VkWindow::VkWindow(i32 w, i32 h, const std::string& name)
		: width(w), height(h), windowName(name)
	{
	
		InitWindow();
	}

	VkWindow::~VkWindow()
	{
		glfwDestroyWindow(window);
	}

	bool VkWindow::IsOpen()
	{
		return !glfwWindowShouldClose(window);
	}
	void VkWindow::FrameBufferResizedCallBack(GLFWwindow* window, i32 w, i32 h)
	{
		auto Window = reinterpret_cast<VkWindow*>(glfwGetWindowUserPointer(window));
		Window->frameBufferResized = true;
		Window->width = w;
		Window->height = h;
	}

	void VkWindow::GetWinSurface(VkInstance inst, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(inst, window, nullptr, surface) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create window surface!");
		}
	}

	void VkWindow::InitWindow()
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

} //namespace engine