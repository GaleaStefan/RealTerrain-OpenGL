#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>
#include <memory>

#include "Camera.h"
#include "Shader.h"
#include "Mesh.h"
#include "GLUtils.h"

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

	double deltaTime;
	double lastFrame;
	double totalTime;
};

