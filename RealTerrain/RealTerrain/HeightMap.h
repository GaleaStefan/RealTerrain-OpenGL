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
	void ReadTerrainConfig(std::ifstream& stream);
	void SmoothTerrain(float factor);
	
public:
	std::vector<std::vector<float>> heightMap;
	std::pair<float, float> verticalBounds;
	std::pair<int, int> size;
	float scale;
	float smoothFactor;
	int smoothSteps;
};

