#pragma once
#include <string>
#include "../types.h"
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
		 static constexpr const wchar* windowName = L"DirectX RayTracer";
			static WindowClass wndClass;
			HINSTANCE hInstance;
		};
	public:
		Window() = delete;
		Window(i32 w, i32 h, const wchar* name) noexcept;
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		~Window();
		[[nodiscard]] HWND GetHWND() const { return hwnd; }
		static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMessageThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		bool ProcessMessages();
	public:
		Keyboard kbd;
	private:
		HWND hwnd;
		i32 width;
		i32 height;
	};
} //namespace dx::engine