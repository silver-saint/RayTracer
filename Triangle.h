#pragma once
#include <memory>
#include <vector>
#include "VkWindow.h"
#include "Pipeline.h"
#include "VkDeviceCtx.h"
#include "VkSwapChain.h"
#include "VkVertexParser.h"
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
		void LoadVertexParser();
		void CreatePipelineLayout();
		void CreatePipeline();
		void CreateCmdBuffers();
		void FreeCmdBuffers();
		void DrawFrame();
		void RecreateSwapChain();
		void RecordCommandBuffer(i32 imgIdx);
		static constexpr ui32 WIDTH = 800;
		static constexpr ui32 HEIGHT = 600;
		VkWindow Window = { WIDTH, HEIGHT, "Triangle" };
		VkDeviceCtx device = {Window};
		std::unique_ptr<VkSwapChain> swapChain;
		std::unique_ptr<Pipeline> Vkpipeline;
		VkPipelineLayout pipelineLayout;
		std::vector<VkCommandBuffer> commandBuffers; 
		std::unique_ptr<VkVertexParser> vertexParser;

	};

}
