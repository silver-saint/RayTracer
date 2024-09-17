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
		window.ProcessMessages();

	}
} // namespace dx::engine;