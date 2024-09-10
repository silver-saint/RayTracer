#pragma once
#include <memory>
#include <vector>
#include "Window.h"
#include "Device.h"
namespace vk::engine
{
		class App
		{
		public:
			App();
			~App();
			App& operator=(const App&) = delete;
			App(const App&) = delete;
			void run();
		private:
			static constexpr ui32 WIDTH = 800;
			static constexpr ui32 HEIGHT = 800;
			Window window{ WIDTH, HEIGHT, L"RayTracer" };
			Device device{ window };
		};

} //namespace vk::engine
