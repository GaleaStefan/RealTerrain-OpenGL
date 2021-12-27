#pragma once

#include "Mesh.h"

class Model
{
public:
    Model(const std::vector<std::shared_ptr<Mesh>>& meshes)
        : meshes(meshes), position(0), rotation(0), scale({ 1, 1, 1 })
    {
    }
    Model(const std::vector<std::shared_ptr<Mesh>>& meshes, const glm::vec3 pos, const glm::vec3 rot,
          const glm::vec3 scale)
        : meshes(meshes), position(pos), rotation(rot), scale(scale)
    {
    }

    const glm::mat4 GetModelMatrix() const;
    void Draw(std::shared_ptr<Shader> shader);

public:
    std::vector<std::shared_ptr<Mesh>> meshes;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
};
