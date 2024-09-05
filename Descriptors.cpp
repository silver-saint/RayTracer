#include "Descriptors.h"
#include <array>
namespace engine
{
	/*
	Descriptors::Descriptors(VkDeviceCtx& deviceRef)
		: device(deviceRef)
	{
		Init();
		CreateDescriptorPool();
		CreateDescriptorSet();
	}

	Descriptors::~Descriptors()
	{
		vkDestroyDescriptorPool(device.GetDevice(), descriptorPool, nullptr);
		vkDestroyDescriptorSetLayout(device.GetDevice(), descriptorSetLayout, nullptr);

	}

	void Descriptors::Init()
	{
		VkDescriptorSetLayoutBinding samplerLayoutBinding {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		std::array<VkDescriptorSetLayoutBinding, 1> bindings = { samplerLayoutBinding };

		VkDescriptorSetLayoutCreateInfo layoutInfo{};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.pNext = nullptr;
		layoutInfo.flags = 0;
		layoutInfo.bindingCount = static_cast<ui32>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(device.GetDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}
	void Descriptors::CreateDescriptorPool()
	{
		std::array<VkDescriptorPoolSize, 1> poolSize {};
		poolSize[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize[0].descriptorCount = static_cast<ui32>(VkSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo {};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
		poolInfo.pPoolSizes = poolSize.data();
		poolInfo.maxSets = static_cast<uint32_t>(VkSwapChain::MAX_FRAMES_IN_FLIGHT);
		if (vkCreateDescriptorPool(device.GetDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create descriptor pool!");
		}

	}
	void Descriptors::CreateDescriptorSet()
	{
		std::vector<VkDescriptorSetLayout> layouts(VkSwapChain::MAX_FRAMES_IN_FLIGHT, descriptorSetLayout);
		VkDescriptorSetAllocateInfo allocInfo {};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(VkSwapChain::MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		descriptorSets.resize(VkSwapChain::MAX_FRAMES_IN_FLIGHT);
		if (vkAllocateDescriptorSets(device.GetDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate descriptor sets!");
		}
		for (size_t i = 0; i < VkSwapChain::MAX_FRAMES_IN_FLIGHT; i++)
		{
		
			VkDescriptorImageInfo imageInfo {};

			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = texImg.GetImageView();
			imageInfo.sampler = sampler.GetTextureSampler();
			std::array<VkWriteDescriptorSet, 1> descriptorWrites {};
			
			descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrites[0].dstSet = descriptorSets[i];
			descriptorWrites[0].dstBinding = 1;
			descriptorWrites[0].dstArrayElement = 0;
			descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrites[0].descriptorCount = 1;
			descriptorWrites[0].pImageInfo = &imageInfo;

			vkUpdateDescriptorSets(device.GetDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}
	}
	*/
} // namespace engine