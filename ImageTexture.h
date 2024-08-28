#pragma once
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <string>
#include "types.h"
#include <vulkan/vulkan.h>
class ImageTexture
{
public:
	ImageTexture(const std::string& path, i32 texW, i32 TexH, i32 TexChannels, i32 RgbType);
	VkDeviceSize GetImageSize() const { return imageSize; }
private:
	void Init();
	const std::string filepath;
	i32 texW;
	i32 texH;
	i32 texChannels;
	i32 rgbType;
	VkDeviceSize imageSize;

};

