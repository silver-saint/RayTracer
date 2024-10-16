#pragma once
#include <string>
#include "../types.h"
#include "../platform/WinInstance.h"
#include "../Keyboard/Keyboard.h"
#include "../dx/Graphics.h"
#include <optional>
#include <memory>
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
		[[nodiscard]] Graphics& Gfx() { return *pGfx; }
		[[nodiscard]] u32 GetWindowWidth() const { return width; }
		[[nodiscard]] u32 GetWindowHeight() const { return height; }
		static LRESULT CALLBACK HandleMessageSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMessageThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static std::optional<i32> ProcessMessages();
	public:
		Keyboard kbd;
	private:
		HWND hwnd;
		u32 width;
		u32 height;
		std::unique_ptr<Graphics> pGfx;
	};
} //namespace dx::engine