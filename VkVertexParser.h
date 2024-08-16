#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include "VkDeviceCtx.h"
namespace engine
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
		static glm::vec3 SetColor(glm::vec3 Color);
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
	};
	class VkVertexParser
	{
	public:
		VkVertexParser() = delete;
		VkVertexParser(VkDeviceCtx& deviceRef, const std::vector<Vertex>& vertices);
		~VkVertexParser();
		VkVertexParser(const VkDeviceCtx&) = delete;
		VkVertexParser& operator=(const VkDeviceCtx&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);
	private:
		void CreateVertexBuffers(const std::vector<Vertex> vertices);
		VkDeviceCtx& device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		ui32 vertexCount;
	};
}
