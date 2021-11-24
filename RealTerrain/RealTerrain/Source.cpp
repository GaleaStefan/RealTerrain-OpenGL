#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(int argc, char** argv)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Game window", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glewInit();

	std::vector<Vertex> verts{
		{{0.f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 1.f, 0.f}, {0.f, 0.f}},
		{{0.5f, 0.f, 0.f}, {0.f, 0.f, 0.f}, {0.f, 0.f, 1.f}, {0.f, 0.f}},
		{{0.5f, 0.5f, 0.f}, {0.f, 0.f, 0.f}, {1.f, 0.f, 0.f}, {0.f, 0.f}}
	};

	std::vector<unsigned int> ind{ 0, 1, 2 };

	Mesh triangle(verts, ind);
	Shader basic("BasicShader.vertex.glsl", "BasicShader.fragment.glsl");

	while (!glfwWindowShouldClose(window)) {
		basic.Use();
		triangle.Draw(basic);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}