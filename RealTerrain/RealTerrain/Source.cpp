#include <iostream>

#include "Application.h"

#pragma comment (lib, "glfw3dll.lib")
#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "OpenGL32.lib")

#define STB_IMAGE_IMPLEMENTATION
#include "../../_external/stb/stb_image.h"
#include "HeightMap.h"
#include "Terrain.h"

int main(int argc, char** argv)
{
	try
	{
		std::unique_ptr<Application> app = std::make_unique<Application>();
		app->Render();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << '\n';
	}

	return 0;
}