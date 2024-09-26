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
		window.Gfx().OnUpdate();
		window.Gfx().OnRender();
		if (window.kbd.KeyIsPressed(VK_SPACE))
		{
			MessageBox(nullptr, L"Kur", L"Mur", 0);
		}
	}

	int App::Run()
	{
		window.Gfx().OnInit();
		while (true)
		{
			if (const auto ecode = window.ProcessMessages())
			{
				return *ecode;
			}
			DoFrame();
		}
		window.Gfx().OnDestroy();
		return 0;
	}
} // namespace dx::engine;