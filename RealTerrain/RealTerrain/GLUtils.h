#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
}

