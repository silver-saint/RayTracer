#pragma once
#include <memory>
#include <vector>
#include "VkWindow.h"
#include "VkDeviceCtx.h"
#include "RenderSystem.h"
#include "Renderer.h"
#include "Menu.h"

namespace engine
{
	class Rectangle
	{
	public:
		Rectangle();
		~Rectangle();
		Rectangle& operator=(const Rectangle&) = delete;
		Rectangle(const Rectangle&) = delete;
		void run();
	private:
		void LoadGameObjects();
		static constexpr ui32 WIDTH = 800;
		static constexpr ui32 HEIGHT = 600;
		VkWindow window = { WIDTH, HEIGHT, "Rectangle" };
		VkDeviceCtx device = { window };
		Renderer renderer = { window, device };
		std::vector<GameObject> gameObjects;

	};

}
