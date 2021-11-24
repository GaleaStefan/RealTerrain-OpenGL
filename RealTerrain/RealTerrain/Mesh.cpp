#include "Mesh.h"

void Mesh::Draw(Shader& shader) const
{
	glBindVertexArray(vertArrObj);
	glDrawElements(GL_TRIANGLES, indicies.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Setup()
{
	glGenVertexArrays(1, &vertArrObj);
	glGenBuffers(1, &vertBuffObj);
	glGenBuffers(1, &elemBuffObj);

	glBindVertexArray(vertArrObj);

	glBindBuffer(GL_ARRAY_BUFFER, vertBuffObj);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	constexpr size_t positionOffset = 0;
	constexpr size_t normalOffset = offsetof(Vertex, normal);
	constexpr size_t colorOffset = offsetof(Vertex, color);
	constexpr size_t textureOffset = offsetof(Vertex, textureUV);
	
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)positionOffset);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)normalOffset);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)colorOffset);

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)textureOffset);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elemBuffObj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicies.size() * sizeof(unsigned int), &indicies[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::Cleanup()
{
	glDeleteVertexArrays(1, &vertArrObj);
	glDeleteBuffers(1, &vertBuffObj);
	glDeleteBuffers(1, &elemBuffObj);
}
