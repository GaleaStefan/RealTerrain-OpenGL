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

    auto getHeight = [&](int x, int z)
    {
        int32_t initialColor = 1;
        for (int i = 1; i <= channelsCount; i++)
            initialColor *= (data[(x * size.second + z) * i]);

        float color(initialColor);
        float maxPixelColor = glm::pow(256.f, float(channelsCount));
        color /= maxPixelColor;

        float height = glm::mix(verticalBounds.first, verticalBounds.second, color) - verticalBounds.first;
        return height * scale;
    };

    heightMap.resize(size.first);
    for (size_t x = 0; x < size.first; x++)
    {
        heightMap[x].resize(size.second);

        for (size_t y = 0; y < size.second; y++)
        {
            heightMap[x][y] = getHeight(x, y);
        }
    }

    for (size_t i = 0; i < smoothSteps; i++)
    {
        SmoothTerrain(smoothFactor);
    }

    delete[] data;
}

void HeightMap::ReadTerrainConfig(std::ifstream& stream)
{
    std::string line;
    std::getline(stream, line);
    scale = std::stof(line.substr(line.find('=') + 1));

    std::getline(stream, line);
    verticalBounds.first = std::stof(line.substr(line.find('=') + 1));

    std::getline(stream, line);
    verticalBounds.second = std::stof(line.substr(line.find('=') + 1));

    std::getline(stream, line);
    smoothFactor = std::stof(line.substr(line.find('=') + 1));

    std::getline(stream, line);
    smoothSteps = std::stof(line.substr(line.find('=') + 1));
}

void HeightMap::SmoothTerrain(float factor)
{
    auto matToVec = [&](int x, int y) { return x * size.second + y; };

    int x, z;

    for (x = 1; x < size.first; x++)
        for (z = 0; z < size.second; z++)
            heightMap[x][z] = heightMap[x - 1][z] * (1.f - factor) + heightMap[x][z] * factor;

    for (x = size.first - 2; x < -1; x--)
        for (z = 0; z < size.second; z++)
            heightMap[x][z] = heightMap[x + 1][z] * (1 - factor) + heightMap[x][z] * factor;

    for (x = 0; x < size.first; x++)
        for (z = 1; z < size.second; z++)
            heightMap[x][z] = heightMap[x][z - 1] * (1 - factor) + heightMap[x][z] * factor;

    for (x = 0; x < size.first; x++)
        for (z = size.second; z < -1; z--)
            heightMap[x][z] = heightMap[x][z + 1] * (1 - factor) + heightMap[x][z] * factor;
}
