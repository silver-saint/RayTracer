#pragma once
#include "types.h"
#include <string>
#include <stdexcept>
#include "WinInstance.h"


namespace vk::engine
{
		class Window
		{
		public:

			Window() = delete;
			Window(i32 w, i32 h, const std::wstring& name) noexcept;
			Window(const Window&) = delete;
			Window& operator=(const Window&) = delete;
			~Window();
		[[nodiscard]] bool IsOpen() const { return isOpen; };
			void PollEvents();
		[[nodiscard]] HINSTANCE GetInstance() const { return hInstance; };
		[[nodiscard]] HWND GetHWND() const { return hwnd; }
		[[nodiscard]] void GetFrameBufferSize(i32& width, i32& height);
		static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		private:
			void InitWindow();
			bool ProcessMessages();
			HWND hwnd;
			HINSTANCE hInstance;
			const std::wstring windowName;
			i32 width;
			i32 height;
			bool isOpen;
			bool frameBufferResized = false;
		};
}
//namespace vk::engine
