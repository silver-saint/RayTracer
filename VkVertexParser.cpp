#include "VkVertexParser.h"
#include <cassert>
#include <cstring>
#include <array>
namespace engine
{
    glm::vec3 Vertex::SetColor(glm::vec3 Color)
    {
        return Color;
    }
    std::vector<VkVertexInputAttributeDescription> Vertex::GetAttributeDescriptions()
    {
        //binding location format offset
        return { {0,0,VK_FORMAT_R32G32_SFLOAT,offsetof(Vertex, pos)}, 
                 {1,0,VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)}};
    }

    std::vector<VkVertexInputBindingDescription> Vertex::GetBindingDescriptions()
    {
        //binding stride input rate
        return { {0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX} };
    }

    VkVertexParser::VkVertexParser(VkDeviceCtx& deviceRef, const std::vector<Vertex>& vertices)
        : device{ deviceRef }
    {
        CreateVertexBuffers(vertices);
    }

    VkVertexParser::~VkVertexParser()
    {
        vkDestroyBuffer(device.GetDevice(), vertexBuffer, nullptr);
        vkFreeMemory(device.GetDevice(), vertexBufferMemory, nullptr);
    }

    void VkVertexParser::Bind(VkCommandBuffer commandBuffer)
    {
        std::array<VkBuffer, 1> buffers = { vertexBuffer };
        std::array<VkDeviceSize, 1> offsets = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers.data(), offsets.data());

    }

    void VkVertexParser::Draw(VkCommandBuffer commandBuffer)
    {
        vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
    }

    void VkVertexParser::CreateVertexBuffers(const std::vector<Vertex> vertices)
    {
        vertexCount = static_cast<ui32>(vertices.size());
        assert(vertexCount >= 3 && "Vertices to create a Triangle must be 3!");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
        device.CreateBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, vertexBuffer, vertexBufferMemory);
        void* data;
        vkMapMemory(device.GetDevice(), vertexBufferMemory, 0, bufferSize, 0, &data);
        std::memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device.GetDevice(), vertexBufferMemory);
    }
}
//namespace engine