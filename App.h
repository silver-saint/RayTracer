#pragma once
#include <memory>
#include <vector>
#include "Window.h"
#include "Device.h"
#include "Renderer.h"
namespace vk::engine
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
			Device device = { window };
			Renderer renderer = { window, device };
		};

} //namespace vk::engine
