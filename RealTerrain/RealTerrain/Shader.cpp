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
		auto vert = LoadShader(vertShaderPath, GL_VERTEX_SHADER);
		auto frag = LoadShader(fragShaderPath, GL_FRAGMENT_SHADER);
		glAttachShader(programId, vert);
		glAttachShader(programId, frag);
		glLinkProgram(programId);
		glDeleteShader(vert);
		glDeleteShader(frag);

		int success;
		char* infoLog = new char[1024];
		glGetProgramiv(programId, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(programId, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: PROGRAM" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
		}
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << '\n';
	}
}

unsigned int Shader::LoadShader(const std::string& fileName, GLenum shaderType)
{
	const char* codeBuffer = ReadFromFile(fileName);
	unsigned int shaderId = 0;

	try
	{
		CompileShader(codeBuffer, shaderId, shaderType);
		delete[] codeBuffer;

		return shaderId;
	}
	catch (std::exception const& e)
	{
		std::cout << e.what() << " |Shader:" << fileName << '\n';
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

	std::copy(codeString.begin(), codeString.end(), code);
	code[codeString.size()] = 0;

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
		std::cerr << infoLog << '\n';
		throw std::runtime_error("Shader compilation error");
	}
}
