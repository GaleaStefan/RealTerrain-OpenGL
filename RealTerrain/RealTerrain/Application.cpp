#include "Application.h"
#include <iostream>
#include <exception>
#include <fstream>
#include "Model.h"

//std::unordered_map<GLFWwindow*, Application*> Application::callbackLinker;

Application::Application()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	std::cout << mode->width << mode->height;

	window = glfwCreateWindow(mode->width, mode->height, "Game window", nullptr, nullptr);
	
	if (!window)
		throw std::exception("Could not create window");

	glfwMakeContextCurrent(window);
	glfwSetWindowUserPointer(window, (void*)this);
	BindCallbacks();
	glewInit();

	playerCam = std::make_shared<Camera>(mode->width, mode->height, glm::vec3{ 0,0,1.f });
	basicShader = std::make_shared<Shader>("BasicShader.vert", "BasicShader.frag");
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

	auto scrollCallback = [](GLFWwindow* window, double x, double y)
	{
		Application* application = static_cast<Application*>(glfwGetWindowUserPointer(window));
		application->OnScroll(x, y);
	};

	glfwSetFramebufferSizeCallback(window, frameBuffResizeCallback);
	glfwSetCursorPosCallback(window, mouseMoveCallback);
	glfwSetScrollCallback(window, scrollCallback);
}

void Application::Render()
{
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	deltaTime = 0;
	lastFrame = 0;
	totalTime = 0;

	std::ifstream test("Models/test.obj");
	auto mesh = util::ParseObj(test);
	std::vector<std::shared_ptr<Mesh>> modelMeshes;
	modelMeshes.push_back(mesh);
	auto model = std::make_shared<Model>(modelMeshes);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		totalTime += deltaTime;

		model->position = glm::vec3(glm::sin(totalTime), glm::cos(totalTime), 0.f);

		basicShader->Use();
		basicShader->SetMat4("model", model->GetModelMatrix());
		basicShader->SetMat4("view", playerCam->GetViewMatrix());
		basicShader->SetMat4("projection", playerCam->GetProjectionMatrix());

		model->Draw(basicShader);

		ProcessKeyboardInput();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::OnFrameBufferResize(int newWidth, int newHeight)
{
	std::cout << "Frame buffer resize:" << newWidth << " " << newHeight << "\n";
	playerCam->Reshape(newWidth, newHeight);
}

void Application::OnMouseMove(float x, float y)
{
	std::cout << "Mouse move:" << x << " " << y << "\n";
	playerCam->Look(x, y);
}

void Application::OnScroll(double x, double y)
{
	std::cout << "Mouse scroll:" << x << " " << y << "\n";
	playerCam->Zoom(y);
}

void Application::ProcessKeyboardInput()
{
	static std::unordered_map<unsigned int, Camera::MoveDirection> keyToDirection
	{
		{GLFW_KEY_W, Camera::MoveDirection::Forward},
		{GLFW_KEY_S, Camera::MoveDirection::Backward},
		{GLFW_KEY_A, Camera::MoveDirection::Left},
		{GLFW_KEY_D, Camera::MoveDirection::Right},
		{GLFW_KEY_SPACE, Camera::MoveDirection::Up},
		{GLFW_KEY_LEFT_SHIFT, Camera::MoveDirection::Down}
	};

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	for (const auto& [key, direction] : keyToDirection)
	{
		if (glfwGetKey(window, key) == GLFW_PRESS)
			playerCam->Move(direction, deltaTime);
	}
}
