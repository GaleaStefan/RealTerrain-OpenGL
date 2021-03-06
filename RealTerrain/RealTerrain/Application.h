#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>
#include <unordered_map>

#include "Camera.h"
#include "GLUtils.h"
#include "Lights.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Texture.h"

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
    // std::shared_ptr<Shader> basicShader;
    std::shared_ptr<Shader> skyboxShader;
    std::shared_ptr<Shader> mainShader;
    std::shared_ptr<Terrain> terrain;
    std::shared_ptr<CubeMap> skybox;
    std::shared_ptr<Mesh> flag;

    int width;
    int height;

    double deltaTime;
    double lastFrame;
    double totalTime;
};
