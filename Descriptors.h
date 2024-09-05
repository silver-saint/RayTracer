#pragma once
#include <vector>
#include "Device.h"
#include "VkSwapChain.h"
#include "TextureImage.h"
#include "TextureSampler.h"
#include "vulkan/vulkan.h"
namespace engine
{
	/*
	class Descriptors
	{
	public:
		Descriptors(VkDeviceCtx& deviceRef);
		~Descriptors();
		const VkDescriptorSetLayout* GetDescriptorSetLayout() const { return &descriptorSetLayout; }
		const VkDescriptorSet* GetDescriptorSetAt(size_t idx) { return &descriptorSets[idx]; }
	private:
		void Init();
		void CreateDescriptorPool();
		void CreateDescriptorSet();
		VkDeviceCtx& device;
		VkDescriptorSetLayout descriptorSetLayout;
		std::vector<VkDescriptorSet> descriptorSets;
		VkDescriptorPool descriptorPool;
		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;
		TextureImage texImg {device, "Textures/cubetexture.png"};
		TextureSampler sampler{ device };
	};
	*/
}