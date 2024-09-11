#include "App.h"

namespace vk::engine
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
			//vkDeviceWaitIdle(device.GetDevice());
		}
} // namespace vk::engine