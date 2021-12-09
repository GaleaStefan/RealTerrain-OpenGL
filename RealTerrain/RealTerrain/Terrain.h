#pragma once
#include <vector>
#include <memory>

#include "TerrainChunk.h"
#include "Shader.h"
#include "HeightMap.h"
#include "Vertex.h"
#include "GLUtils.h"

#include <unordered_map>

class Terrain
{
public:
	void Draw(std::shared_ptr<Shader> shader, const glm::vec3& from) const;
	void Generate(const HeightMap& map);

	float HeightAt(float worldX, float worldZ) const;
private:
	std::unordered_map<std::pair<int, int>, std::shared_ptr<TerrainChunk>, util::pair_hash> chunks;
	glm::vec3 position;
	int terrainSize;
	int chunkSize;
};

