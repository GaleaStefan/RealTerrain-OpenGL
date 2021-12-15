#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <memory>

#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "GLUtils.h"
#include "Terrain.h"
#include "Lights.h"

class Application
{
public:
	Application();
	~Application() { glfwTerminate(); }

	void BindCallbacks();
	void Render();

	void OnFrameBufferResize(int newWidth, int newHeight);
	void OnMouseMove(float x, float y);
	void OnScroll(double x, double y);
	void ProcessKeyboardInput();

private:
	GLFWwindow* window = nullptr;

	std::shared_ptr<Camera> playerCam;
	std::shared_ptr<Shader> basicShader;
	std::shared_ptr<Shader> skyboxShader;
	std::shared_ptr<Terrain> terrain;
	std::shared_ptr<CubeMap> skybox;

	int width;
	int height;

	double deltaTime;
	double lastFrame;
	double totalTime;
};

