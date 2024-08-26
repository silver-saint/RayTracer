#include "Triangle.h"
#include <stdexcept>
#include <array>
namespace engine
{
	void Triangle::run()
	{
		RenderSystem renderSystem = {device, renderer.GetSwapChainRenderPass()};
		while (window.IsOpen())
		{
			glfwPollEvents();
			if (auto commandbuffer = renderer.BeginFrame())
			{
				renderer.BeginSwapChainRenderPass(commandbuffer);
				renderSystem.RenderObjects(commandbuffer, gameObjects);
				renderer.EndSwapChainRenderPass(commandbuffer);
				renderer.EndFrame();
			}
		}
		vkDeviceWaitIdle(device.GetDevice());
	}

	void Triangle::LoadGameObjects()
	{

		Builder builder;
		static Menu menu;
		menu.Init();
		switch (menu.GetInput())
		{
		case 1:
		{
			builder.vertices =
			{
			   {{-1.0f, 1.0f}, { 1.0f, 1.0f, 0.0f }},
			   {{-0.5f, 0.0f}, { 1.0f, 1.0f, 0.0f }},
			   {{0.0f, 1.0f},  { 1.0f, 1.0f, 0.0f }},
			   {{0.5f, 0.0f}, { 1.0f, 1.0f, 0.0f }},
			   {{1.0f, 1.0f}, { 1.0f, 1.0f, 0.0f }},
			   {{0.0f, -1.0f}, {1.0f, 1.0f, 0.0f}}
			};
			builder.indicies = { 0,1,2,2,3,4,1,5,3 };
			break;
		}
		case 2:
		{
			builder.vertices =
			{
			   {{-0.5f, -0.5f}, { 1.0f, 1.0f, 0.0f }},
			   {{0.5f, -0.5f}, { 1.0f, 1.0f, 0.0f }},
			   {{0.5f, 0.5f},  { 1.0f, 1.0f, 0.0f }},
			   {{-0.5f, 0.5f}, { 1.0f, 1.0f, 0.0f }},
			};
			builder.indicies = { 0,1,2,2,3,0 };
			break;
		}
		default:
		{
			builder.vertices =
			{
			   {{0.0f, -0.5f}, { 1.0f, 0.0f, 0.0f }},
			   {{0.5f, 0.5f}, { 0.0f, 1.0f, 0.0f }},
			   {{-0.5f, 0.5f},  { 0.0f, 0.0f, 1.0f }}
			};
			builder.indicies = { 0,1,2 };
			break;
		}
		}
		auto modelVertices = std::make_shared<VkModel>(device, builder);

		auto triangle = GameObject::CreateGameObject();
		triangle.parser = modelVertices;
		gameObjects.push_back(std::move(triangle));
	}
	
	Triangle::Triangle()
	{
		LoadGameObjects();
		
	}

	Triangle::~Triangle() {}
}
//namespace engine