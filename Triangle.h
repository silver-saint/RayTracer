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
	class Triangle
	{
	public:
		Triangle();
		~Triangle();
		Triangle& operator=(const Triangle&) = delete;
		Triangle(const Triangle&) = delete;
		void run();
	private:
		void LoadGameObjects();
		static constexpr ui32 WIDTH = 800;
		static constexpr ui32 HEIGHT = 600;
		VkWindow window = { WIDTH, HEIGHT, "Triangle" };
		VkDeviceCtx device = {window};
		Renderer renderer = { window, device };
		std::vector<GameObject> gameObjects;

	};

}
