#pragma once
#include "VkWindow.h"
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
	};

}
