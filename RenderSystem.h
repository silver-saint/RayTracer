#pragma once

#include "Device.h"
#include "Pipeline.h"
#include "GameObject.h"
#include "Descriptors.h"
#include <memory>
#include <vector>

namespace engine 
{
	/*
	struct PushConstantData {
		glm::vec2 offset;
		alignas(16) glm::vec3 color;
		//glm::mat4 modelMatrix{ 1.f };
		//glm::mat4 normalMatrix{ 1.f };
	};

	class RenderSystem {
	public:
		RenderSystem(VkDeviceCtx& deviceCtxRef, VkRenderPass renderPass, Descriptors& descriptorRef);
		~RenderSystem();

		RenderSystem(const RenderSystem&) = delete;
		RenderSystem& operator=(const RenderSystem&) = delete;
		void RenderObjects(VkCommandBuffer cmdbuffer, std::vector<GameObject>& gameObjects);
		VkPipelineLayout GetPipelineLayout() const { return pipelineLayout; }
	private:
		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
		VkDeviceCtx& device;
		Descriptors& descriptors;
		std::unique_ptr<Pipeline> Vkpipeline;
		VkPipelineLayout pipelineLayout;
		
	};
	*/
}  // namespace engine