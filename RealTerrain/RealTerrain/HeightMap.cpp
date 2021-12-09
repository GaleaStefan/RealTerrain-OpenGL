#include "HeightMap.h"
#include "../../_external/stb/stb_image.h"

#include <glm.hpp>

void HeightMap::LoadFrom(const std::string& name)
{
	std::string image = name + ".png";
	std::ifstream config(name + ".cfg");
	ReadTerrainConfig(config);

	int channelsCount = 0;
	stbi_set_flip_vertically_on_load(false);
	uint8_t* data = stbi_load(image.c_str(), &size.second, &size.first, &channelsCount, 0);

	heightMap.resize(size.first);
	for (size_t x = 0; x < size.first; x++)
	{
		heightMap[x].resize(size.second);

		for (size_t y = 0; y < size.second; y++)
		{
			uint8_t grayScale = data[(x * size.second + y) * channelsCount];
			heightMap[x][y] = PixelToHeight(grayScale) - verticalBounds.first;
		}
	}

	delete[] data;
}

void HeightMap::ReadTerrainConfig(std::ifstream& stream)
{
	constexpr float TO_METERS = 10.f;
	std::string line;
	std::getline(stream, line);
	scale = std::stof(line.substr(line.find('=') + 1));

	std::getline(stream, line);
	verticalBounds.first = std::stof(line.substr(line.find('=') + 1)) * scale * TO_METERS;

	std::getline(stream, line);
	verticalBounds.second = std::stof(line.substr(line.find('=') + 1)) * scale * TO_METERS;
}

float HeightMap::PixelToHeight(uint8_t pixel)
{
	return glm::mix(verticalBounds.first, verticalBounds.second, (float)pixel / 255.f);
}
