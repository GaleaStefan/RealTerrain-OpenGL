#include "HeightMap.h"
#include "../../_external/stb/stb_image.h"

#include <glm.hpp>

void HeightMap::LoadFrom(const std::string& name)
{
	std::string image = name + ".png";
	std::ifstream config(name + ".cfg");
	verticalBounds = ReadTerrainVerticalBounds(config);

	int channelsCount = 0;
	uint8_t* pixels = stbi_load(image.c_str(), &size.first, &size.second, &channelsCount, 0);

	heightMap.resize(size.first);
	for (size_t x = 0; x < size.first; x++)
	{
		heightMap[x].resize(size.second);

		for (size_t y = 0; y < size.second; y++)
		{
			heightMap[x][y] = PixelToHeight(pixels[size.first * x + y], verticalBounds);
		}
	}

	delete[] pixels;
}

std::pair<float, float> HeightMap::ReadTerrainVerticalBounds(std::ifstream& stream)
{
	std::pair<float, float> bounds;

	std::string line;
	std::getline(stream, line);
	bounds.first = std::stof(line.substr(line.find('=') + 1));

	std::getline(stream, line);
	bounds.second = std::stof(line.substr(line.find('=') + 1));

	return bounds;
}

float HeightMap::PixelToHeight(uint8_t pixel, const std::pair<float, float>& bounds)
{
	return glm::mix(bounds.first, bounds.second, (float)pixel / 255.f);
}
