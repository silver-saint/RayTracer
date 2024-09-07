#include "Window.h"

namespace vk::engine
{
		Window::Window(i32 w, i32 h, const std::string& name)
			: width(w), height(h), windowName(name)
		{

			InitWindow();
		}

		Window::~Window()
		{
			SDL_DestroyWindow(window);
			SDL_Quit();
		}
		void Window::PollEvents()
		{
			SDL_Event e;
			while (SDL_PollEvent(&e) != 0)
			{
				switch (e.type)
				{
				case SDL_QUIT: isOpen = false;
					break;
				}
			}
		}
		void Window::InitWindow()
		{
			if (SDL_Init(SDL_INIT_EVERYTHING) != NULL)
			{
				throw std::runtime_error("Cannot init SDL");
			}
			window = SDL_CreateWindow(windowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_VULKAN);
			if (!window)
			{
				throw std::runtime_error("Error, couldn't init SDL Window");
			}
			isOpen = true;
		}
} //namespace vk::engine