#pragma once
#include <glm/glm.hpp>
#include "VkDeviceCtx.h"
namespace engine
{
	struct Vertex
	{
		glm::vec2 pos;
		glm::vec3 color;
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
	};
	class VertexParser
	{
	public:
		VertexParser() = delete;
		VertexParser(VkDeviceCtx& deviceRef, const std::vector<Vertex>& vertices);
		~VertexParser();
		VertexParser(const VkDeviceCtx&) = delete;
		VertexParser& operator=(const VkDeviceCtx&) = delete;

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
