#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>
#include "types.h"
#include <string>
#include <stdexcept>



namespace vk::engine
{
		class Window
		{
		public:

			Window() = delete;
			Window(i32 w, i32 h, const std::string& name);
			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;
			~Window();
		[[nodiscard]] bool IsOpen() const { return isOpen; };
			void PollEvents();
		[[nodiscard]] SDL_Window* GetWindow() { return window; };
		private:
			void InitWindow();
			const std::string windowName;
			i32 width;
			i32 height;
			SDL_Window* window;
			bool isOpen;
			bool frameBufferResized = false;
		};
}
//namespace vk::engine
