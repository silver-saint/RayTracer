#include "TextureImageView.h"
#include <stdexcept>
namespace engine
{
	TextureImageView::TextureImageView(VkDeviceCtx& deviceRef, VkImage texImage, VkImageView texImageView, VkDeviceMemory texMemory)
		:device(deviceRef), textureImage(texImage), textureImageView(texImageView), textureImageMemory(texMemory)
	{
		CreateImageView();
	}
	VkImageView TextureImageView::CreateImageView(VkDevice device, VkImage image, VkFormat format)
	{
		VkImageViewCreateInfo viewInfo{};
		viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		viewInfo.image = image;
		viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		viewInfo.format = format;
		viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		viewInfo.subresourceRange.baseMipLevel = 0;
		viewInfo.subresourceRange.levelCount = 1;
		viewInfo.subresourceRange.baseArrayLayer = 0;
		viewInfo.subresourceRange.layerCount = 1;

		VkImageView imageView;
		if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create texture image view!");
		}
		return imageView;

	}

} // namespace engine