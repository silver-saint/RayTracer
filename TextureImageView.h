#pragma once
#include "VkDeviceCtx.h"
#include "TextureImage.h"
#include <vulkan/vulkan.h>
namespace engine
{
	class TextureImageView
	{
	public:
		TextureImageView(VkDeviceCtx& deviceRef, VkImageView texImageView);
		static VkImageView CreateImageView(VkDevice device, VkImage image, VkFormat format);
		VkImageView GetImageView() const { return textureImageView; }
	private:
		VkDeviceCtx& device;
		VkImageView textureImageView;
	};
} //namespace engine
