#define STB_IMAGE_IMPLEMENTATION
#include "ImageTexture.h"
#include <stdexcept>

namespace engine
{
	ImageTexture::ImageTexture(VkDeviceCtx& deviceRef, const std::string& path, const i32 TexW, const i32 TexH)
		: 
		device(deviceRef),
		filepath(path),
		texH(TexH),
		texW(TexW),
		imageSize(texH* texW * 4)
	{
		Init();
	}

	ImageTexture::~ImageTexture()
	{

		vkDestroyImageView(device.GetDevice(), textureImageView, nullptr);
		vkDestroyImage(device.GetDevice(), textureImage, nullptr);
		vkFreeMemory(device.GetDevice(), textureImageMemory, nullptr);
	}

	void ImageTexture::CreateTextureImageView()
	{
		textureImageView = CreateImageView(device.GetDevice(), textureImage, VK_FORMAT_R8G8B8A8_SRGB);
	}

	VkImageView ImageTexture::CreateImageView(VkDevice device, VkImage image, VkFormat format)
	{
		VkImageViewCreateInfo viewInfo {};
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

	void ImageTexture::Init()
	{
		i32 texChannels;
		pixels = stbi_load(filepath.c_str(), &texW, &texH, &texChannels, STBI_rgb_alpha);

		if (!pixels)
		{
			throw std::runtime_error("failed to load texture image!");
		}

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		device.CreateBuffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(device.GetDevice(), stagingBufferMemory, 0, imageSize, 0, &data);
		memcpy(data, pixels, static_cast<size_t>(imageSize));
		vkUnmapMemory(device.GetDevice(), stagingBufferMemory);

		stbi_image_free(pixels);

		device.CreateImage(texW, texH, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, textureImage, textureImageMemory);

		device.TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
		device.CopyBufferToImage(stagingBuffer, textureImage, static_cast<uint32_t>(texW), static_cast<uint32_t>(texH), 1);
		device.TransitionImageLayout(textureImage, VK_FORMAT_R8G8B8A8_SRGB, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		vkDestroyBuffer(device.GetDevice(), stagingBuffer, nullptr);
		vkFreeMemory(device.GetDevice(), stagingBufferMemory, nullptr);
	}
} // namespace engine