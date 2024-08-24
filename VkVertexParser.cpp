#include "VkVertexParser.h"
#include <cassert>
#include <cstring>
#include <array>
namespace engine
{
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

    VkVertexParser::VkVertexParser(VkDeviceCtx& deviceRef, const Builder& builder)
        : device{ deviceRef }
    {
        CreateVertexBuffers(builder.vertices);
        CreateIndexBuffers(builder.indicies);
    }

    VkVertexParser::~VkVertexParser()
    {
        vkDestroyBuffer(device.GetDevice(), vertexBuffer, nullptr);
        vkFreeMemory(device.GetDevice(), vertexBufferMemory, nullptr);

        if (hasIndexBuffer)
        {
            vkDestroyBuffer(device.GetDevice(), indexBuffer, nullptr);
            vkFreeMemory(device.GetDevice(), indexBufferMemory, nullptr);
        }
    }

    void VkVertexParser::Bind(VkCommandBuffer commandBuffer)
    {
        std::array<VkBuffer, 1> buffers = { vertexBuffer };
        std::array<VkDeviceSize, 1> offsets = { 0 };
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers.data(), offsets.data());

        if (hasIndexBuffer)
        {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }

    }

    void VkVertexParser::Draw(VkCommandBuffer commandBuffer)
    {
        if (hasIndexBuffer)
        {
            vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void VkVertexParser::CreateVertexBuffers(const std::vector<Vertex>& vertices)
    {
        vertexCount = static_cast<ui32>(vertices.size());
        assert(vertexCount >= 3 && "Vertices to create a Triangle must be 3!");
        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;
      //  bufferSize = bufferSize / 4096 + (bufferSize % 4096 ? 4096 : 0);
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        device.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        
        void* data;
        vkMapMemory(device.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        std::memcpy(data, vertices.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device.GetDevice(), stagingBufferMemory);
        
        device.CreateBuffer(bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);
        device.CopyBuffer(stagingBuffer, vertexBuffer, bufferSize);
      
        vkDestroyBuffer(device.GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(device.GetDevice(), stagingBufferMemory, nullptr);

    }
    void VkVertexParser::CreateIndexBuffers(const std::vector<ui32>& indicies)
    {
        indexCount = static_cast<ui32>(indicies.size());
        hasIndexBuffer = indexCount > 0;
        if (!hasIndexBuffer)
        {
            return;
        }
        VkDeviceSize bufferSize = sizeof(indicies[0]) * indexCount;
       // bufferSize = bufferSize / 4096 + (bufferSize % 4096 ? 4096 : 0);
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        
        device.CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        
        void* data;
        vkMapMemory(device.GetDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
        std::memcpy(data, indicies.data(), static_cast<size_t>(bufferSize));
        vkUnmapMemory(device.GetDevice(), stagingBufferMemory);
        
        device.CreateBuffer(bufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);
        device.CopyBuffer(stagingBuffer, indexBuffer, bufferSize);
       
        vkDestroyBuffer(device.GetDevice(), stagingBuffer, nullptr);
        vkFreeMemory(device.GetDevice(), stagingBufferMemory, nullptr);
    }
}
//namespace engine