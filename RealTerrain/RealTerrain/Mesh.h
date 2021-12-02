#pragma once
#include "Vertex.h"
#include "Shader.h"
#include <vector>
#include <memory>

class Mesh
{
public:
	Mesh(const std::vector<Vertex> verts, const std::vector<unsigned int> indicies) :
		vertices(verts), indicies(indicies) {
		Setup();
	}

	~Mesh() { Cleanup(); }

	void Draw(std::shared_ptr<Shader> shader) const;

private:
	void Setup();
	void Cleanup();
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indicies;
	unsigned int vertArrObj, vertBuffObj, elemBuffObj;
};

