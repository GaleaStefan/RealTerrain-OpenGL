#pragma once
#include <memory>
#include <vector>

#include "GLUtils.h"
#include "HeightMap.h"
#include "Shader.h"
#include "TerrainChunk.h"
#include "Vertex.h"

#include <unordered_map>

class Terrain
{
public:
    void Draw(std::shared_ptr<Shader> shader, const glm::vec3& from) const;
    void Generate(const HeightMap& map);

    float HeightAt(float worldX, float worldZ) const;

private:
    std::unordered_map<std::pair<int, int>, std::shared_ptr<TerrainChunk>, util::pair_hash> chunks;
    int terrainSize;
    int chunkSize = 32;
    int drawRadius = 16;
};
