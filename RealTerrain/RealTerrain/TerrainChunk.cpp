#include "TerrainChunk.h"

std::shared_ptr<TerrainChunk> TerrainChunk::CreateFrom(const std::vector<Vertex>& vertices, const std::pair<int, int> chunkSize)
{
	auto matToVec = [&chunkSize](const std::pair<size_t, size_t>& pos) -> size_t
	{
		return pos.first * chunkSize.second + pos.second;
	};

    std::vector<unsigned int> indicies;

	for (size_t j = 0; j < chunkSize.second - 1; j++)
	{
		for (size_t i = 0; i < chunkSize.first - 1; i++)
		{
			indicies.push_back(matToVec({ i ,j }));
			indicies.push_back(matToVec({ i, j + 1 }));
			indicies.push_back(matToVec({ i + 1 ,j }));

			indicies.push_back(matToVec({ i ,j + 1 }));
			indicies.push_back(matToVec({ i + 1 ,j + 1 }));
			indicies.push_back(matToVec({ i + 1 ,j }));
		}
	}

	std::shared_ptr<TerrainChunk> terrainChunk = std::make_shared<TerrainChunk>();
	terrainChunk->mesh = std::make_shared<Mesh>(vertices, indicies);

	return terrainChunk;
}

void TerrainChunk::Draw(std::shared_ptr<Shader> shader)
{
	mesh->Draw(shader);
}
