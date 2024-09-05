#pragma once
#include <stb_image.h>
#include <string>
#include "types.h"
#include "Device.h"
#include <vulkan/vulkan.h>
namespace engine
{
	/*
	class TextureImage
	{
	public:
		TextureImage(VkDeviceCtx& deviceRef, const std::string& path);
		~TextureImage();
		TextureImage(const TextureImage&) = delete;
		TextureImage& operator=(const TextureImage&) = delete;
		stbi_uc* GetSrcPixels() const { return pixels; }
		void FreeImage() { stbi_image_free(pixels); }
		static VkImageView CreateImageView(VkDevice device,VkImage image, VkFormat format);
	private:
		void Init();
		void CreateTextureImageView();
		const std::string filepath;
		stbi_uc* pixels;
		VkDeviceCtx& device;
		VkImage textureImage;
		VkImageView textureImageView;
		VkDeviceMemory textureImageMemory;
	};
	*/
}
