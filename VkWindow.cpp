#include "vkWindow.h"

engine::VkWindow::VkWindow(ui32 w, ui32 h, const std::string& name)
	: width(w), height(h), windowName(name) 
{
	InitWindow();
}

engine::VkWindow::~VkWindow()
{
	glfwDestroyWindow(window);
}

bool engine::VkWindow::IsOpen()
{
	return !glfwWindowShouldClose(window);
}

void engine::VkWindow::GetWinSurface(VkInstance inst, VkSurfaceKHR* surface)
{
	if (glfwCreateWindowSurface(inst, window, nullptr, surface) != VK_SUCCESS)
	{
		throw std::runtime_error("failed to create window surface!");
	}
}

void engine::VkWindow::InitWindow()
{
	if (!glfwInit())
	{
		throw std::runtime_error("Error, couldn't init GLFW");
	}
	
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (!window)
	{
		throw std::runtime_error("Error, couldn't init Window.");
	}
}
