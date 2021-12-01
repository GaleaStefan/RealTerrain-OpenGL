#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <unordered_map>

class Application
{
public:
	Application(int width, int height, const char* winTitle, GLFWmonitor* monitor, GLFWwindow* share);
	~Application() { glfwTerminate(); }

	void SetActive();
	void BindCallbacks();
	void Render();

	void OnFrameBufferResize(int newWidth, int newHeight);
	void OnMouseMove(float x, float y);
//private:
//	static std::unordered_map<GLFWwindow*, Application*> callbackLinker;
private:
	GLFWwindow* window = nullptr;
};

