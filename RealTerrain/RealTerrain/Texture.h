#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include <memory>

#include "Camera.h"
#include "Shader.h"

struct CubeMap
{
	unsigned int textureId;
	unsigned int mapVAO, mapVBO;
	bool loaded;
	static float skyboxVertices[108];

	CubeMap() : textureId(0), mapVAO(), mapVBO(), loaded(false) {}

	void Load(const std::vector<std::string>& images);
	void Draw(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader) const;
};
