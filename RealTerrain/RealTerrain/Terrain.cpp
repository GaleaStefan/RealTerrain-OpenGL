#include "Terrain.h"
#include "../../_external/color/src/color/color.hpp"

void Terrain::Draw(std::shared_ptr<Shader> shader, const glm::vec3& from) const
{
    std::pair<int, int> currentLocation = { from.x / chunkSize, from.z / chunkSize };

    for (const auto& [location, chunk] : chunks)
    {
        if (glm::pow(currentLocation.first - location.first, 2) +
                glm::pow(currentLocation.second - location.second, 2) <=
            glm::pow(drawRadius, 2))
            chunk->Draw(shader);
    }
}

void Terrain::Generate(const HeightMap& map)
{
    terrainSize = map.size.first;

    std::vector<Vertex> vertices;
    vertices.reserve(map.size.first * map.size.second);

    auto getHeight = [](int x, int y, const HeightMap& map) -> float
    {
        if (x < 0 || x >= map.size.first || y < 0 || y >= map.size.second)
            return 0.f;

        return map.heightMap[x][y];
    };

    // Generate all vertices
    for (size_t x = 0; x < map.size.first; x++)
    {
        for (size_t z = 0; z < map.size.second; z++)
        {
            float heightNormalized = map.heightMap[x][z] / (map.verticalBounds.second - map.verticalBounds.first);

            float hue = (float)(x * map.size.second + z) / (map.size.first * map.size.second) * 360.f;
            color::hsv<float> hsv({hue, 50.f, 50.f});
            glm::vec3 color(color::get::red(hsv), color::get::green(hsv), color::get::blue(hsv));

            vertices.push_back({ { x, map.heightMap[x][z], z },
                                 { heightNormalized, 1.f - heightNormalized, 1.f },
                                 color,
                                 { (float)x / (float)map.size.first, (float)z / (float)map.size.second } });
        }
    }

    // Calculate normals
    for (size_t x = 0; x < map.size.first; x++)
    {
        for (size_t z = 0; z < map.size.second; z++)
        {
            float heightLeft = getHeight(x, z - 1, map);
            float heightRight = getHeight(x, z + 1, map);
            float heightTop = getHeight(x - 1, z, map);
            float heightBottom = getHeight(x + 1, z, map);

            int index = x * map.size.second + z;
            vertices[index].normal =
                glm::normalize(glm::vec3{ heightBottom - heightTop, 2.f, heightLeft - heightRight });
        }
    }

    // Split to chuncks
    for (int chunkRow = 0; chunkRow < map.size.first / chunkSize; chunkRow++)
    {
        for (int chunkCol = 0; chunkCol < map.size.second / chunkSize; chunkCol++)
        {
            std::vector<Vertex> chunkVertices;
            std::tuple<int, int, int, int> chunkBounds = { glm::clamp(chunkSize * chunkRow - 1, 0, map.size.first),
                                                           (int)std::min(map.size.first, chunkSize * (chunkRow + 1)),
                                                           glm::clamp(chunkSize * chunkCol - 1, 0, map.size.second),
                                                           (int)std::min(map.size.second, chunkSize * (chunkCol + 1)) };

            for (int x = std::get<0>(chunkBounds); x < std::get<1>(chunkBounds); x++)
                for (int z = std::get<2>(chunkBounds); z < std::get<3>(chunkBounds); z++)
                    chunkVertices.push_back(vertices[x * map.size.second + z]);

            chunks[{ chunkRow, chunkCol }] =
                TerrainChunk::CreateFrom(chunkVertices, { std::get<1>(chunkBounds) - std::get<0>(chunkBounds),
                                                          std::get<3>(chunkBounds) - std::get<2>(chunkBounds) });
        }
    }
}

float Terrain::HeightAt(float worldX, float worldZ) const
{
    float offsetX = worldX;
    float offsetZ = worldZ;

    if (offsetX < 0 || offsetX >= terrainSize || offsetZ < 0 || offsetZ >= terrainSize)
        return 0;

    int chunkX = glm::ceil(offsetX) / chunkSize;
    int chunkZ = glm::ceil(offsetZ) / chunkSize;

    int gridX = (int)(offsetX) % chunkSize;
    int gridZ = (int)(offsetZ) % chunkSize;

    float whole;
    float xCoord = glm::modf(offsetX, whole);
    float zCoord = glm::modf(offsetZ, whole);

    auto chunk = chunks.at({ chunkX, chunkZ });
    auto chunkSize = chunk->size;

    std::vector<float> corners = { chunk->mesh->vertices[gridX * chunkSize.second + gridZ].position.y,
                                   chunk->mesh->vertices[gridX * chunkSize.second + gridZ + 1].position.y,
                                   chunk->mesh->vertices[(gridX + 1) * chunkSize.second + gridZ].position.y,
                                   chunk->mesh->vertices[(gridX + 1) * chunkSize.second + gridZ + 1].position.y };

    if (xCoord <= (1 - zCoord))
        return util::BarryCentric({ 0, corners[0], 0 }, { 1, corners[2], 0 }, { 0, corners[1], 1 }, { xCoord, zCoord });

    return util::BarryCentric({ 1, corners[2], 0 }, { 1, corners[3], 1 }, { 0, corners[1], 1 }, { xCoord, zCoord });
}
