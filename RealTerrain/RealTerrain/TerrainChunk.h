#pragma once

#include <vector>
#include <memory>

#include "Vertex.h"
#include "Mesh.h"

class TerrainChunk
{
public:
	static std::shared_ptr<TerrainChunk> CreateFrom(const std::vector<Vertex>& vertices, const std::pair<int, int> chunkSize);
	void Draw(std::shared_ptr<Shader> shader);
private:
	std::shared_ptr<Mesh> mesh;
	std::pair<int, int> size;

private:
	friend class Terrain;
};

