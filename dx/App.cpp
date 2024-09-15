#include "App.h"
namespace dx::engine
{
	App::App()
	{
		run();
	}
	App::~App() {}

	void App::run()
	{

		while (window.IsOpen())
		{
			window.PollEvents();
		}
	}
} // namespace dx::engine;