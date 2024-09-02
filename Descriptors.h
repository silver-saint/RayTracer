#pragma once
#include <vector>
#include "VkDeviceCtx.h"
#include "VkSwapChain.h"
#include "ImageTexture.h"
#include "TextureSampler.h"
#include "vulkan/vulkan.h"
namespace engine
{
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
		ImageTexture imgTex{ device, "Textures/cubetexture.png", 836, 900 };
		TextureSampler sampler{ device };
	};

}