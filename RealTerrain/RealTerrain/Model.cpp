#include "Model.h"
#include <ext/matrix_transform.hpp>

const glm::mat4 Model::GetModelMatrix() const
{
    glm::mat4 mat(1.f);
    mat = glm::scale(mat, scale);
    mat = glm::rotate(mat, rotation.x, { 1.f, 0.f, 0.f });
    mat = glm::rotate(mat, rotation.y, { 0.f, 1.f, 0.f });
    mat = glm::rotate(mat, rotation.z, { 0.f, 0.f, 1.f });
    mat = glm::translate(mat, position);

    return mat;
}

void Model::Draw(std::shared_ptr<Shader> shader)
{
    for (const auto& mesh : meshes)
        mesh->Draw(shader);
}
