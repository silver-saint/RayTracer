#include "../Window/Window.h"

namespace dx::engine
{
	Window::WindowClass Window::WindowClass::wndClass;

	Window::WindowClass::WindowClass()
		:hInstance{ GetModuleHandle(nullptr) }
	{

		windowName = L"RayTracer";
		WNDCLASSEX winClass;
		winClass = {};
		winClass.cbSize = sizeof(winClass);
		winClass.lpszClassName = GetName();
		winClass.lpfnWndProc = nullptr;
		winClass.cbClsExtra = 0;
		winClass.cbWndExtra = 0;
		winClass.hIcon = nullptr;
		winClass.hCursor = nullptr;
		winClass.hbrBackground = nullptr;
		winClass.lpszMenuName = nullptr;
		winClass.hIconSm = nullptr;
		winClass.style = CS_VREDRAW | CS_HREDRAW;
		winClass.hInstance = GetInstance();
		RegisterClassEx(&winClass);
	}
	HINSTANCE Window::WindowClass::GetInstance() noexcept
	{
		return wndClass.hInstance;
	}
	const wchar* Window::WindowClass::GetName() noexcept
	{
		return windowName;
	}
	Window::WindowClass::~WindowClass()
	{
		UnregisterClass(wndClass.windowName, GetInstance());
	}
	Window::Window(i32 w, i32 h, const wchar* name) noexcept
		: width(w), height(h)
	{

		InitWindow();
	}

	Window::~Window()
	{
		DestroyWindow(hwnd);
	}
	void Window::PollEvents()
	{
		if (ProcessMessages() == 0)
		{
			isOpen = false;
		}
	}
	bool Window::ProcessMessages()
	{
		MSG msg = {};
		while (GetMessage(&msg, nullptr, 0, 0) > 0)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			
		}
		return msg.wParam;
	}
	LRESULT Window::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		switch (msg)
		{
		case WM_CLOSE:
			if (MessageBox(hwnd, L"Do you really want to close?", L"RayTracer", MB_OKCANCEL) == IDOK)
			{
				PostQuitMessage(0);
				return 0;
			}
			break;
		case WM_KEYDOWN: kbd.onKeyPressed(static_cast<ui8>(wParam));
			break;
		case WM_KEYUP: kbd.onKeyReleased(static_cast<ui8>(wParam));
			break;
		case WM_CHAR: kbd.onChar(static_cast<ui8>(wParam));
			break;
		default: return DefWindowProc(hwnd, msg, wParam, lParam);
		}
		return -1;
	}
	void Window::InitWindow()
	{
		RECT windowRect = {};
		windowRect.left = 100;
		windowRect.bottom = 100;
		windowRect.right = windowRect.left + width;
		windowRect.top = windowRect.bottom + height;
		hwnd = CreateWindowEx(0, windowName.c_str(), windowName.c_str(), WS_OVERLAPPEDWINDOW,
			CW_USEDEFAULT, CW_USEDEFAULT, windowRect.right - windowRect.left, windowRect.top - windowRect.bottom, nullptr, nullptr,
			hInstance, nullptr);
		if (!hwnd)
		{
			MessageBox(hwnd, L"Couldn't init hwnd", L"Error!", MB_OK);
			throw std::runtime_error("Hwnd initialization failure");
		}
		isOpen = true;
		ShowWindow(hwnd, SW_SHOW);
	}
	const wchar* Window::WindowClass::GetName() noexcept
	{
		return nullptr;
	}
	HINSTANCE Window::WindowClass::GetInstance() noexcept
	{
		return HINSTANCE();
	}
	Window::WindowClass::~WindowClass()
	{
	}
}
