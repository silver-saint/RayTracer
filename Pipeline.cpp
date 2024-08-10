#include "Pipeline.h"
#include <fstream>
#include <stdexcept>
#include <print>
engine::Pipeline::Pipeline(const std::string& vertexFP, const std::string& fragmentFP)
{
	CreateGraphicsPipeLine(vertexFP, fragmentFP);
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

void engine::Pipeline::CreateGraphicsPipeLine(const std::string& vertexFP, const std::string& fragmentFP)
{
	auto vertex = readFile(vertexFP);
	auto fragment = readFile(fragmentFP);
	std::println("{0}", vertex.size());
	std::println("{0}", fragment.size());
}
