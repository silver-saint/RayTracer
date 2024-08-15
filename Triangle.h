#pragma once
#include <memory>
#include <vector>
#include "VkWindow.h"
#include "Pipeline.h"
#include "VkDeviceCtx.h"
#include "VkSwapChain.h"
#include "VertexParser.h"
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
		void LoadVertexParser();
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCmdBuffers();
		void DrawFrame();
		static constexpr ui32 WIDTH = 800;
		static constexpr ui32 HEIGHT = 600;
		VkWindow Window = { WIDTH, HEIGHT, "Triangle" };
		VkDeviceCtx device = {Window};
		VkSwapChain swapChain = { device, Window.GetExtent() };
		std::unique_ptr<Pipeline> Vkpipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers; 
		std::unique_ptr<VertexParser> vertexParser;
	};

}
