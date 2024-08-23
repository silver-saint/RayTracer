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
		static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions();
		static std::vector<VkVertexInputBindingDescription> GetBindingDescriptions();
	};
	struct Builder
	{
		std::vector<Vertex> vertices = {};
		std::vector<ui32> indicies = {};
	};
	class VkVertexParser
	{
	public:
		VkVertexParser() = delete;
		VkVertexParser(VkDeviceCtx& deviceRef, const Builder& builder);
		~VkVertexParser();
		VkVertexParser(const VkDeviceCtx&) = delete;
		VkVertexParser& operator=(const VkDeviceCtx&) = delete;

		void Bind(VkCommandBuffer commandBuffer);
		void Draw(VkCommandBuffer commandBuffer);
	private:
		void CreateVertexBuffers(const std::vector<Vertex> &vertices);
		void CreateIndexBuffers(const std::vector<ui32> &indicies);
		VkDeviceCtx& device;
		VkBuffer vertexBuffer;
		VkDeviceMemory vertexBufferMemory;
		ui32 vertexCount;

		VkBuffer indexBuffer;
		VkDeviceMemory indexBufferMemory;
		ui32 indexCount;
		bool hasIndexBuffer = false;

	};
}
