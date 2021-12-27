#pragma once

#include <memory>
#include <vector>

#include "Mesh.h"
#include "Vertex.h"

class TerrainChunk
{
public:
    static std::shared_ptr<TerrainChunk> CreateFrom(const std::vector<Vertex>& vertices,
                                                    const std::pair<int, int> chunkSize);
    void Draw(std::shared_ptr<Shader> shader);

private:
    std::shared_ptr<Mesh> mesh;
    std::pair<int, int> size;

private:
    friend class Terrain;
};
