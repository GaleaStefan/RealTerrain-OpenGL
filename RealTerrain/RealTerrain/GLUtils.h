#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <fstream>
#include <memory>
#include <string>
#include <sstream>
#include <vector>
#include <functional>

#include "Mesh.h"


namespace local
{
	std::vector<std::string> __split_string(const std::string& str, char delimiter);
}

namespace util
{
	template<typename ...Args>
	inline constexpr void GlEnable(Args ...arguments)
	{
		for (GLenum x : { arguments... })
			glEnable(x);
	}

	template<typename ...Args>
	inline constexpr void GlDisable(Args ...arguments)
	{
		for (GLenum x : { arguments... })
			glDisable(x);
	}

	std::shared_ptr<Mesh> ParseObj(std::ifstream& file);
}

