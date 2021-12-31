#pragma once
#include <memory>
#include <vector>

#include "GLUtils.h"
#include "HeightMap.h"
#include "Shader.h"
#include "TerrainChunk.h"
#include "Vertex.h"

#include "../../_external/color/src/color/color.hpp"

#include <unordered_map>

class Terrain
{
public:
    void Draw(std::shared_ptr<Shader> shader, const glm::vec3& from) const;
    void Generate(const HeightMap& map);

    float HeightAt(float worldX, float worldZ) const;

private:
    color::rgb<float> GetColor(float height) const;
    std::unordered_map<std::pair<int, int>, std::shared_ptr<TerrainChunk>, util::pair_hash> chunks;
    int terrainSize;
    int chunkSize = 32;
    int drawRadius = 32;

    struct TerrainType
    {
        float height;
        color::rgb<float> color;
    };

    const std::vector<TerrainType> terrainTypes{
        { 40.f, color::rgb<float>{ 0.21f, 0.21f, 0.21f } },
        // { .0f, color::rgb<float>{ 0.27f, 0.5f, 0.07f } },
        { 80.f, color::rgb<float>{ 0.18f, 0.32f, 0.07f } },
        { 150.f, color::rgb<float>{ 0.36f, 0.26f, 0.23f } },
        { 250.f, color::rgb<float>{ 0.29f, 0.24f, 0.23f } },
        { 500.f, color::rgb<float>{ 0.7f, 0.7f, 0.7f } },
    };
};
