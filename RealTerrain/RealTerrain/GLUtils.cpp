#include "GLUtils.h"

namespace local
{
	std::vector<std::string> __split_string(const std::string& str, char delimiter)
	{
		std::istringstream iss(str);
		std::vector<std::string> result;
		std::string token;
		while (std::getline(iss, token, delimiter))
			result.push_back(token);

		return result;
	}
}

namespace util
{
	std::shared_ptr<Mesh> ParseObj(std::ifstream& file)
	{
		file.seekg(0);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textureUVs;
		std::vector<unsigned int> indicies;
		std::vector<Vertex> verticies;

		while (!file.eof())
		{
			std::string line;
			std::getline(file, line);

			if (line[0] == '#' || line[0] == 's' ||
				line.substr(0, 6) == "usemtl" || line.substr(0, 6) == "mtllib")
				continue;

			auto tokens = local::__split_string(line, ' ');
			auto identifier = tokens[0];

			if (identifier == "v")
			{
				positions.push_back({ std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) });
				continue;
			}

			if (identifier == "vt")
			{
				textureUVs.push_back({ std::stof(tokens[1]), std::stof(tokens[2]) });
				continue;
			}

			if (identifier == "vn")
			{
				normals.push_back({ std::stof(tokens[1]), std::stof(tokens[2]), std::stof(tokens[3]) });
				continue;
			}

			if (identifier != "f")
				continue;

			for (size_t tIndex = 1; tIndex <= 3; tIndex++)
			{
				auto elements = local::__split_string(tokens[tIndex], '/');
				std::size_t pos = std::stoi(elements[0]) - 1, tex = std::stoi(elements[1]) - 1, norm = std::stoi(elements[2]) - 1;
				verticies.push_back({ positions[pos], normals[norm], {1.f, 1.f, 1.f}, textureUVs[tex] });
				indicies.push_back(verticies.size() - 1);
			}
		}

		return std::make_shared<Mesh>(verticies, indicies);
	}
}