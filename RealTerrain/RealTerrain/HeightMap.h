#pragma once
#include <vector>
#include <string>
#include <fstream>

class HeightMap
{
public:
	HeightMap() : heightMap(), verticalBounds(), size() {}
	void LoadFrom(const std::string& name);

private:
	std::pair<float, float> ReadTerrainVerticalBounds(std::ifstream& stream);
	float PixelToHeight(uint8_t pixel, const std::pair<float, float>& bounds);
	
private:
	std::vector<std::vector<float>> heightMap;
	std::pair<float, float> verticalBounds;
	std::pair<int, int> size;
};

