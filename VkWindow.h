#pragma once
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include "types.h"
#include <string>
#include <print>


namespace engine
{

	class VkWindow
	{
	public:
		
		VkWindow() = delete;
		VkWindow(ui32 w, ui32 h, const std::string name);
		VkWindow(const VkWindow&) = delete;
		VkWindow& operator=(const VkWindow&) = delete;
		~VkWindow();
		bool IsOpen();
	private:
		void InitWindow();
		const std::string windowName;
		ui32 width;
		ui32 height;
		GLFWwindow* window;
	};
}
