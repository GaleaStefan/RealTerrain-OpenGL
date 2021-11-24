#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

void Shader::Use() const
{
	glUseProgram(programId);
}

void Shader::SetVec3(const std::string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void Shader::Load(const std::string& vertShaderPath, const std::string& fragShaderPath)
{
	programId = glCreateProgram();

	try
	{
		LoadShader(vertShaderPath, GL_VERTEX_SHADER);
		LoadShader(fragShaderPath, GL_FRAGMENT_SHADER);
	}
	catch (std::exception(e))
	{
		std::cout << e.what();
	}
}

void Shader::LoadShader(const std::string& fileName, GLenum shaderType)
{
	const char* codeBuffer = ReadFromFile(fileName);
	unsigned int shaderId = 0;

	try
	{
		CompileShader(codeBuffer, shaderId, shaderType);
		glAttachShader(shaderId, programId);
		glDeleteShader(shaderId);
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << " |Shader:" << fileName;
		throw;
	}
}

const char* Shader::ReadFromFile(const std::string& file)
{
	std::ifstream stream;
	stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	stream.open(file);
	std::stringstream codeStream;
	codeStream << stream.rdbuf();

	auto codeString = codeStream.str();
	char* code = new char[codeString.size() + 1];
	std::memcpy(code, codeString.c_str(), codeString.size() + 1);
	return code;
}

void Shader::CompileShader(const char* code, unsigned int& newShaderId, GLenum shaderType)
{
	newShaderId = glCreateShader(shaderType);
	glShaderSource(newShaderId, 1, &code, NULL);
	glCompileShader(newShaderId);

	int success;
	char* infoLog = new char[1024];
	glGetShaderiv(newShaderId, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(newShaderId, 1024, NULL, infoLog);
		std::cout << infoLog;
		throw std::exception("Shader compilation error");
	}
}
