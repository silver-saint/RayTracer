#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include "types.h"
#include "VkDeviceCtx.h"
#include <vulkan/vulkan.h>
namespace engine
{
	class ImageTexture
	{
	public:
		ImageTexture(VkDeviceCtx& deviceRef, const std::string& path, i32 texW, i32 TexH, i32 TexChannels);
		~ImageTexture();
		ImageTexture(const ImageTexture&) = delete;
		ImageTexture& operator=(const ImageTexture&) = delete;
		VkDeviceSize GetImageSize() const { return imageSize; }
		stbi_uc* GetSrcPixels() const { return pixels; }
		void FreeImage() { stbi_image_free(pixels); }
		i32 GetTexW() const { return texW; }
		i32 GetTexH() const { return texH; }
	private:
		void Init();
		const std::string filepath;
		i32 texW;
		i32 texH;
		i32 texChannels;
		VkDeviceSize imageSize;
		stbi_uc* pixels;
		VkDeviceCtx& device;
		VkImage textureImage;
		VkDeviceMemory textureImageMemory;
	};
}
