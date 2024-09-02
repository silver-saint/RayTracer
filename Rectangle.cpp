#include "Rectangle.h"

engine::Rectangle::Rectangle()
{
	LoadGameObjects();
}

engine::Rectangle::~Rectangle() {}

void engine::Rectangle::run()
{
	RenderSystem renderSystem = { device, renderer.GetSwapChainRenderPass(), descriptor};
	while (window.IsOpen())
	{
		glfwPollEvents();
		if (auto commandbuffer = renderer.BeginFrame())
		{
			renderer.BeginSwapChainRenderPass(commandbuffer);
			vkCmdBindDescriptorSets(commandbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderSystem.GetPipelineLayout(), 0, 1, descriptor.GetDescriptorSetAt(renderer.GetFrameIndex()), 0, nullptr);
			renderSystem.RenderObjects(commandbuffer, gameObjects);
			renderer.EndSwapChainRenderPass(commandbuffer);
			renderer.EndFrame();
		}
	}
	vkDeviceWaitIdle(device.GetDevice());
}

void engine::Rectangle::LoadGameObjects()
{
	Builder builder;
	builder.vertices =
	{
	   {{-0.5f, -0.5f}, { 1.0f, 1.0f, 0.0f }, {1.0f, 0.0f}},
	   {{0.5f, -0.5f}, { 1.0f, 1.0f, 0.0f }, {0.0f, 0.0f}},
	   {{0.5f, 0.5f},  { 1.0f, 1.0f, 0.0f }, {0.0f, 1.0f}},
	   {{-0.5f, 0.5f}, { 1.0f, 1.0f, 0.0f }, {1.0f, 1.0f}},
	};
	builder.indicies = { 0,1,2,2,3,0 };


	auto modelVertices = std::make_shared<VkModel>(device, builder);

	auto rectangle = GameObject::CreateGameObject();
	rectangle.parser = modelVertices;
	gameObjects.push_back(std::move(rectangle));
}
