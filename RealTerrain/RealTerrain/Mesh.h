#pragma once
#include "Shader.h"
#include "Vertex.h"
#include <memory>
#include <vector>

class Mesh
{
public:
    Mesh(const std::vector<Vertex> verts, const std::vector<unsigned int> indicies)
        : vertices(verts), indicies(indicies)
    {
        Setup();
    }

    ~Mesh() { Cleanup(); }

    void Draw(std::shared_ptr<Shader> shader) const;

private:
    void Setup();
    void Cleanup();

public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indicies;
    unsigned int vertArrObj, vertBuffObj, elemBuffObj;
};
