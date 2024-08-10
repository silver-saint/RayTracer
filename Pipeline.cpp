#include "Pipeline.h"
#include <fstream>
#include <stdexcept>
#include <print>
engine::Pipeline::Pipeline(VkDeviceCtx& deviceRef, const std::string& vertexFP, const std::string& fragmentFP, const PipeLineConfigInfo& configInfo)
	: device {deviceRef}
{
	CreateGraphicsPipeLine(vertexFP, fragmentFP, configInfo);
}

engine::PipeLineConfigInfo engine::Pipeline::DefaultPipeLineConfigInfo(ui32 w, ui32 h)
{
	PipeLineConfigInfo configInfo = {};
	return configInfo;
}

std::vector<char> engine::Pipeline::readFile(const std::string& fp)
{
	std::ifstream file(fp, std::ios::ate | std::ios::binary);
	if (!file.is_open())
	{
		throw std::runtime_error("failed to open the file " + fp);
	}
	size_t fileSize = static_cast<size_t>(file.tellg());
	std::vector<char> buffer(fileSize);
	file.seekg(0);
	file.read(buffer.data(), fileSize);

	return buffer;
}

void engine::Pipeline::CreateShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule)
{
	VkShaderModuleCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	createInfo.codeSize = code.size();
	createInfo.pCode = reinterpret_cast<const ui32*>(code.data());

	if (vkCreateShaderModule(device.GetDevice(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
	{
		throw std::runtime_error("Cannot create shader module");
	}
}

void engine::Pipeline::CreateGraphicsPipeLine(const std::string& vertexFP, const std::string& fragmentFP, const PipeLineConfigInfo& configInfo)
{
	auto vertex = readFile(vertexFP);
	auto fragment = readFile(fragmentFP);
}
