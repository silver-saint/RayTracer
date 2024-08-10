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


	configInfo.inputAssemblyInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	configInfo.inputAssemblyInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	configInfo.inputAssemblyInfo.primitiveRestartEnable = VK_FALSE;


	configInfo.viewport.x = 0.0f;
	configInfo.viewport.y = 0.0f;
	configInfo.viewport.width = static_cast<f32>(w);
	configInfo.viewport.height = static_cast<f32>(h);
	configInfo.viewport.minDepth = 0.0f;
	configInfo.viewport.maxDepth = 1.0f;
	
	configInfo.scissor.offset = { 0,0 };
	configInfo.scissor.extent = { w,h };

	configInfo.viewportInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	configInfo.viewportInfo.pNext = nullptr;
	configInfo.viewportInfo.flags = 0;
	configInfo.viewportInfo.viewportCount = 1;
	configInfo.viewportInfo.pViewports = &configInfo.viewport;
	configInfo.viewportInfo.pScissors = &configInfo.scissor;

	configInfo.raserizationInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	configInfo.raserizationInfo.depthClampEnable = VK_FALSE;
	configInfo.raserizationInfo.rasterizerDiscardEnable = VK_FALSE;
	configInfo.raserizationInfo.polygonMode = VK_POLYGON_MODE_FILL;
	configInfo.raserizationInfo.lineWidth = 1.0f;
	configInfo.raserizationInfo.cullMode = VK_CULL_MODE_NONE;
	configInfo.raserizationInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	configInfo.raserizationInfo.depthBiasClamp = VK_FALSE;
	configInfo.raserizationInfo.depthBiasEnable = VK_FALSE;
	configInfo.raserizationInfo.depthBiasConstantFactor = 0.0f;
	configInfo.raserizationInfo.depthBiasClamp = 0.0f;
	configInfo.raserizationInfo.depthBiasSlopeFactor = 0.0f;

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
