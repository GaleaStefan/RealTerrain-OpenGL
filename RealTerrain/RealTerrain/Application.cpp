#include "Application.h"
#include <iostream>
#include <exception>
#include <fstream>
#include "Model.h"
#include "HeightMap.h"
#include "Terrain.h"

Application::Application()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	window = glfwCreateWindow(mode->width, mode->height, "Game window", nullptr, nullptr);
	
	if (!window)
		throw std::exception("Could not create window");

	width = mode->width;
	height = mode->height;

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
		application->OnMouseMove((float)x, (float)y);
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
	util::GlEnable(GL_CULL_FACE);

	deltaTime = 0;
	lastFrame = 0;
	totalTime = 0;

	/*std::ifstream test("Models/test.obj");
	auto mesh = util::ParseObj(test);
	std::vector<std::shared_ptr<Mesh>> modelMeshes;
	modelMeshes.push_back(mesh);
	auto model = std::make_shared<Model>(modelMeshes);*/

	HeightMap hmap;
	hmap.LoadFrom("heightmap");
	Terrain terrain;
	terrain.Generate(hmap);

	playerCam->camSpeed = 30.f;

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		double currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		totalTime += deltaTime;

		basicShader->Use();
		basicShader->SetMat4("model", glm::mat4(1));
		basicShader->SetMat4("view", playerCam->GetViewMatrix());
		basicShader->SetMat4("projection", playerCam->GetProjectionMatrix());

		terrain.Draw(basicShader, glm::vec3{ 0.f ,0.f, 0.f });

		ProcessKeyboardInput();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void Application::OnFrameBufferResize(int newWidth, int newHeight)
{
	playerCam->Reshape(newWidth, newHeight);
	width = newWidth;
	height = newHeight;
}

void Application::OnMouseMove(float x, float y)
{
	float warpPoint = 30.f;
	auto warp = [warpPoint](float current, float size)
	{
		if (current <= warpPoint)
			return size - 2 * warpPoint;

		if (current >= size - warpPoint)
			return warpPoint * 2;

		return current;
	};

	float newX = warp(x, width);

	if (x != newX)
	{
		playerCam->lastX = newX;
		glfwSetCursorPos(window, newX, y);
		return;
	}
	playerCam->Look(x, y);
}

void Application::OnScroll(double x, double y)
{
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
