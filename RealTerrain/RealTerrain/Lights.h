#pragma once

#include <glm.hpp>

struct DiffuseLight
{
    glm::vec3 position;
    glm::vec3 color;
};

struct DirectionalLight
{
    glm::vec3 direction;
    glm::vec3 color;
};