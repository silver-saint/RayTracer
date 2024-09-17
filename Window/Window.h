#pragma once
#include <string>
#include <stdexcept>
#include "../platform/WinInstance.h"
#include "../Keyboard/Keyboard.h"

namespace dx::engine
{
	class Window
	{
		
	private:
		class WindowClass
		{
		public:
			static const wchar* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;

		private:
			WindowClass() noexcept;
			~WindowClass();
			WindowClass& operator=(const WindowClass&) = delete;
			WindowClass(const WindowClass&) = delete;
			static const wchar* windowName;
			static WindowClass wndClass;
			HINSTANCE hInstance;
		};
	public:
		Window() = delete;
		Window(i32 w, i32 h, const wchar* name) noexcept;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();
		[[nodiscard]] bool IsOpen() const { return isOpen; };
		void PollEvents();
		[[nodiscard]] HWND GetHWND() const { return hwnd; }
		static LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static Keyboard kbd;
	private:
		void InitWindow();
		bool ProcessMessages();
		HWND hwnd;
		i32 width;
		i32 height;
		bool isOpen;
	};
} //namespace dx::engine