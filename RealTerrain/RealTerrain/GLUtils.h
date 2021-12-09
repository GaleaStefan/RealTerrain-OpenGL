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

	struct pair_hash
	{
		template <class T1, class T2>
		inline std::size_t operator() (const std::pair<T1, T2>& pair) const {
			return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
		}
	};

	float BarryCentric(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec2& pos);
}

