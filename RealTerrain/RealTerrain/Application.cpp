#include "Application.h"
#include <exception>

//std::unordered_map<GLFWwindow*, Application*> Application::callbackLinker;

Application::Application(int width, int height, const char* winTitle, GLFWmonitor* monitor, GLFWwindow* share)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "Game window", monitor, share);
	
	if (!window)
		throw std::exception("Could not create window");

	//callbackLinker[window] = this;
	glewInit();
}

void Application::SetActive()
{
	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, (void*)this);
}

void Application::BindCallbacks()
{
	auto frameBuffResizeCallback = [](GLFWwindow* window, int width, int height)
	{
		Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
		application->OnFrameBufferResize(width, height);
	};

	auto mouseMoveCallback = [](GLFWwindow* window, double x, double y)
	{
		Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
		application->OnMouseMove(x, y);
	};

	glfwSetFramebufferSizeCallback(window, frameBuffResizeCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
}

void Application::Render()
{
	SetActive();
	BindCallbacks();

	while (!glfwWindowShouldClose(window))
	{
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::OnFrameBufferResize(int newWidth, int newHeight)
{
}

void Application::OnMouseMove(float x, float y)
{
}
