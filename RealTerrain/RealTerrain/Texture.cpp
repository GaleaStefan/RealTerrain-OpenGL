#include "Texture.h"
#include "../../_external/stb/stb_image.h"

#include <stdexcept>

float CubeMap::skyboxVertices[108] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

void CubeMap::Load(const std::vector<std::string>& images)
{
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

	int width, height, channels;
	GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
	
	for (const auto& image : images)
	{
		uint8_t* data = stbi_load(image.c_str(), &width, &height, &channels, 0);

		if (channels != 3)
			throw std::runtime_error(("Invalid cubemap face " + image + "! Expected RGB, got" + std::to_string(channels)).c_str());
		
		if (!data)
			throw std::runtime_error("Could not read cubemap face");

		glTexImage2D(face++, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenVertexArrays(1, &mapVAO);
    glGenBuffers(1, &mapVBO);
    glBindBuffer(GL_ARRAY_BUFFER, mapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

    glBindVertexArray(mapVAO);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
	
    loaded = true;
}

void CubeMap::Draw(std::shared_ptr<Camera> camera, std::shared_ptr<Shader> shader) const
{
    if (!loaded)
        throw std::runtime_error("Cubemap not loaded");

    /*glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);*/
    shader->Use();
    glBindVertexArray(mapVAO);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);

    glm::mat4 view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
    shader->SetMat4("projection", camera->GetProjectionMatrix());
    shader->SetMat4("view", view);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    /*glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);*/
}
