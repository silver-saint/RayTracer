#pragma once
#include "VkWindow.h"
#include "Pipeline.h"
#include "VkDeviceCtx.h"
namespace engine
{
	class Triangle
	{
	public:
		void run();
		~Triangle();
	private:
		static constexpr ui32 WIDTH = 800;
		static constexpr ui32 HEIGHT = 600;
		VkWindow Window = { WIDTH, HEIGHT, "Triangle" };
		VkDeviceCtx device = {Window};
		Pipeline Pipeline = { device, "Shaders/vert.spv", "Shaders/frag.spv", Pipeline::DefaultPipeLineConfigInfo(WIDTH, HEIGHT)};
	};

}
