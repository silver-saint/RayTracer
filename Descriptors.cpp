#include "Descriptors.h"
#include <array>
namespace engine
{
	DescriptorsSetLayout::DescriptorsSetLayout(VkDeviceCtx& deviceRef)
		: device(deviceRef)
	{
		Init();
	}

	DescriptorsSetLayout::~DescriptorsSetLayout()
	{
		vkDestroyDescriptorSetLayout(device.GetDevice(), descriptorSetLayout, nullptr);
	}

	void DescriptorsSetLayout::Init()
	{
		VkDescriptorSetLayoutBinding samplerLayoutBinding{};
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
	void DescriptorsSetLayout::CreateDescriptorPool()
	{
		std::array<VkDescriptorPoolSize, 1> poolSize {};
		poolSize[0].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSize[0].descriptorCount = static_cast<ui32>(VkSwapChain::MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSize.size());
		poolInfo.pPoolSizes = poolSize.data();
		poolInfo.maxSets = static_cast<uint32_t>(VkSwapChain::MAX_FRAMES_IN_FLIGHT);
	}
} // namespace engine