#include "Descriptors.h"

DescriptorsSetLayout::DescriptorsSetLayout()
{
	VkDescriptorSetLayoutCreateInfo descriptorSet {};
	descriptorSet.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	descriptorSet.pNext = nullptr;
	descriptorSet.flags = 0;
	descriptorSet.bindingCount = 0;
	descriptorSet.pBindings = nullptr;
}
