#include "Window.h"

namespace vk::engine
{
		Window::Window(i32 w, i32 h, const std::wstring& name) noexcept
		: width(w), height(h), windowName(name),
			hInstance(GetModuleHandle(nullptr)) 
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
			while (GetMessage(&msg, nullptr, 0,0) > 0)
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return msg.wParam;
		}
		void Window::GetFrameBufferSize(i32& width, i32& height)
		{
			RECT rect;
			GetClientRect(hwnd, &rect);
			width = rect.right - rect.left;
			height = rect.bottom - rect.top;
		}
		LRESULT Window::WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CLOSE: 
				if(MessageBox(hwnd, L"Do you really want to close?", L"RayTracer", MB_OKCANCEL) == IDOK)
				{
					PostQuitMessage(0);
					return 0;
				}
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

			WNDCLASSEX winClass;
			winClass = {};
			winClass.cbSize = sizeof(winClass);
			winClass.lpszClassName = windowName.c_str();
			winClass.lpfnWndProc = WinProc;
			winClass.cbClsExtra = 0;
			winClass.cbWndExtra = 0;
			winClass.hIcon = nullptr;
			winClass.hCursor = nullptr;
			winClass.hbrBackground = nullptr;
			winClass.lpszMenuName = nullptr;
			winClass.hIconSm = nullptr;
			winClass.style = CS_VREDRAW | CS_HREDRAW;
			winClass.hInstance = hInstance;
			RegisterClassEx(&winClass);
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
} //namespace vk::engine