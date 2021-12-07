#pragma once
#include <vector>
#include <memory>

#include "TerrainChunk.h"
#include "Shader.h"
#include "HeightMap.h"

class Terrain
{
public:
	void Render(std::shared_ptr<Shader> shader, const glm::vec3& from) const;
	void Generate(const HeightMap& map);

private:
	std::vector<TerrainChunk> chunks;
};

