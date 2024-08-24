#pragma once

#include "VkDeviceCtx.h"
#include "Pipeline.h"
#include "RenderSystem.h"
#include <memory>
#include <vector>

namespace engine 
{
	class RenderSystem {
	public:
		RenderSystem(VkDeviceCtx& deviceCtxRef, VkRenderPass renderPass);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;


	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);

		VkDeviceCtx& device;

		std::unique_ptr<Pipeline> Vkpipeline;
		VkPipelineLayout pipelineLayout;
	};
}  // namespace lve