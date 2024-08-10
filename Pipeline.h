#pragma once
#include <string>
#include <vector>
namespace engine
{
	class Pipeline
	{
	public:
		Pipeline(const std::string& vertexFP, const std::string& fragmentFP);
	private:
		static std::vector<char> readFile(const std::string& fp);
		void CreateGraphicsPipeLine(const std::string& vertexFP, const std::string& fragmentFP);
	};
}
