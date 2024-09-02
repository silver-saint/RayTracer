#pragma once
#include <vector>
#include "VkDeviceCtx.h"
#include "VkSwapChain.h"
#include "vulkan/vulkan.h"
namespace engine
{
	class DescriptorsSetLayout
	{
	public:
		DescriptorsSetLayout(VkDeviceCtx& deviceRef);
		~DescriptorsSetLayout();
		VkDescriptorSetLayout GetDescriptorSetLayout() const { return descriptorSetLayout; }
	private:
		void Init();
		void CreateDescriptorPool();
		VkDeviceCtx& device;
		VkDescriptorSetLayout descriptorSetLayout;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
	};

}