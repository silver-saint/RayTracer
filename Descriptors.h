#pragma once
#include <vector>
#include "vulkan/vulkan.h"
class DescriptorsSetLayout
{
public:
	DescriptorsSetLayout();
private:
	VkDescriptorSetLayout descriptorSetLayout;
	std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
};

