#include <iostream>

#include "Mesh.h"
#include "GLUtils.h"
#include "Application.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")


int main(int argc, char** argv)
{
	try
	{
		std::unique_ptr<Application> app = std::make_unique<Application>(800, 600, "Game window", nullptr, nullptr);
		app->SetActive();
		app->BindCallbacks();
		app->Render();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << '\n';
	}

	return 0;
}