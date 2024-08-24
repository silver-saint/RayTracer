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
				renderer.EndSwapChainRenderPass(commandbuffer);
				renderer.EndFrame();
			}
		}
		vkDeviceWaitIdle(device.GetDevice());
	}

	void Triangle::LoadVertexParser()
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
		vertexParser = std::make_unique<VkVertexParser>(device, builder);
	}
	
	Triangle::Triangle()
	{
		LoadVertexParser();
	}

	Triangle::~Triangle() {}
}
//namespace engine