#pragma once
#include "Window.h"
namespace dx::engine
{
	class App
	{
	public:
		App();
		~App();
		App& operator=(const App&) = delete;
		App(const App&) = delete;
	private:
		void run();
		static constexpr ui32 WIDTH = 800;
		static constexpr ui32 HEIGHT = 800;
		Window window = { WIDTH, HEIGHT, L"RayTracer" };
	};
} // namespace dx::engine