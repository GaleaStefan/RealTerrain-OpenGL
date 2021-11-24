#pragma once
#include <string>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>

class Shader
{
public:
	Shader(const std::string& vertShaderPath, const std::string& fragShaderPath) { Load(vertShaderPath, fragShaderPath); };
	~Shader() { glDeleteProgram(programId); }

    void Use() const;

	unsigned int GetProgramId() const noexcept { return programId; }

	void SetVec3(const std::string& name, const glm::vec3& value) const;
	void SetVec3(const std::string& name, float x, float y, float z) const;
	void SetMat4(const std::string& name, const glm::mat4& mat) const;
	void SetFloat(const std::string& name, float value) const;

private:
	void Load(const std::string& vertShaderPath, const std::string& fragShaderPath);

	void LoadShader(const std::string& file, GLenum shaderType);

	const char* ReadFromFile(const std::string& file);

	void CompileShader(const char* code, unsigned int& newShaderId, GLenum shaderType);

private:
    unsigned int programId;
};
