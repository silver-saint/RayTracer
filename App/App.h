#pragma once
#include "../Window/Window.h"
#include "../types.h"
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
		int Run();
		void DoFrame();
		static constexpr u32 WIDTH = 800;
		static constexpr u32 HEIGHT = 800;
		Window window;
	};
} // namespace dx::engine