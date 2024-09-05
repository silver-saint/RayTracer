#include "App.h"

namespace engine
{
	namespace vk
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
				glfwPollEvents();
			}
			//vkDeviceWaitIdle(device.GetDevice());
		}

	} // namespace vk
} // namespace engine