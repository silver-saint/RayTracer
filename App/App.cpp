#include "App.h"
namespace dx::engine
{
	App::App()
		: window { WIDTH, HEIGHT, L"RayTracer" }
	{
		Run();
	}
	App::~App() {}

	void App::DoFrame()
	{
//		window.Gfx().Draw(window.GetHWND(), window.GetWindowWidth(), window.GetWindowHeight());
		if (window.kbd.KeyIsPressed(VK_SPACE))
		{
			MessageBox(nullptr, L"Kur", L"Mur", 0);
		}
	}

	int App::Run()
	{
		while (true)
		{
			if (const auto ecode = window.ProcessMessages())
			{
				return *ecode;
			}
			DoFrame();
		}
		return 0;
	}
} // namespace dx::engine;