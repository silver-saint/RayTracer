#include "ImageTexture.h"
#include <stdexcept>
ImageTexture::ImageTexture(const std::string& path, const i32 TexW, const i32 TexH, const i32 TexChannels, const i32 RgbType)
	: filepath(path), 
	  texH(TexH),
	  texW(TexW), 
	  texChannels(TexChannels),
	  rgbType(RgbType),
	  imageSize(texH * texW * 4)
{
	Init();
}

void ImageTexture::Init()
{
	stbi_uc* pixels = stbi_load(filepath.c_str(), &texW, &texH, &texChannels, rgbType);

	if (!pixels)
	{
		throw std::runtime_error("failed to load texture image!");
	}
}
