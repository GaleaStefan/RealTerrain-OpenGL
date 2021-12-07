#include "Terrain.h"

void Terrain::Draw(std::shared_ptr<Shader> shader, const glm::vec3& from) const
{
	for (const auto& [location, chunk] : chunks)
		chunk->Draw(shader);
}

void Terrain::Generate(const HeightMap& map)
{
	std::pair<float, float> center{ map.size.first / 2.f, map.size.second / 2.f };
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
			vertices.push_back({
				{x - center.first, map.heightMap[x][z], z - center.second},
				{1.f, 1.f, 1.f},
				{1.f, 1.f, 1.f},
				{(float)x / (float)map.size.first, (float)z / (float)map.size.second} });
		}
	}

	// Calculate normals
	for (size_t x = 0; x < map.size.first; x++)
	{
		for (size_t z = 0; z < map.size.second; z++)
		{
			float heightLeft = getHeight(x - 1, z, map);
			float heightRight = getHeight(x + 1, z, map);
			float heightTop = getHeight(x, z - 1, map);
			float heightBottom = getHeight(x, z + 1, map);
			vertices[x * map.size.first + z].normal = glm::normalize(glm::vec3{heightRight - heightLeft, heightTop - heightBottom, 2.f});
		}
	}

	constexpr int MAX_CHUNK_SIZE = 32;

	for (size_t chunkCol = 0; chunkCol < map.size.second / MAX_CHUNK_SIZE; chunkCol++)
	{
		for (size_t chunkRow = 0; chunkRow < map.size.first / MAX_CHUNK_SIZE; chunkRow++)
		{
			std::vector<Vertex> chunkVertices;
			std::tuple<size_t, size_t, size_t, size_t> chunkBounds
				= std::make_tuple<size_t, size_t, size_t, size_t>
				(
					(size_t)MAX_CHUNK_SIZE * chunkRow,
					(size_t)std::min<int>(map.size.first, MAX_CHUNK_SIZE * (chunkRow + 1)),
					(size_t)MAX_CHUNK_SIZE * chunkCol,
					(size_t)std::min<int>(map.size.first, MAX_CHUNK_SIZE * (chunkCol + 1))
					);

			for (size_t x = std::get<0>(chunkBounds); x < std::get<1>(chunkBounds); x++)
			{
				for (size_t z = std::get<2>(chunkBounds); z < std::get<3>(chunkBounds); z++)
				{
					chunkVertices.push_back(vertices[x * map.size.second + z]);
				}
			}

			chunks[std::make_pair<int, int>((int)chunkRow, (int)chunkCol)] 
				= TerrainChunk::CreateFrom(chunkVertices, 
					{ std::get<1>(chunkBounds) - std::get<0>(chunkBounds), 
					std::get<3>(chunkBounds) - std::get<2>(chunkBounds) });
		}
	}

}
