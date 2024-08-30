#pragma once
#include "VkDeviceCtx.h"
#include <vulkan/vulkan.h>
namespace engine
{
	class TextureSampler
	{
	public:
		TextureSampler(VkDeviceCtx& deviceRef);
		~TextureSampler();
		TextureSampler(const TextureSampler&) = delete;
		TextureSampler& operator=(const TextureSampler&) = delete;
	private:
		void Init();
		VkDeviceCtx& device;
		VkSampler textureSampler;
	};

}