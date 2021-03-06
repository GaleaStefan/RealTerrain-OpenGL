#include "Application.h"
#include "HeightMap.h"
#include "Model.h"
#include <ext/matrix_transform.hpp>
#include <fstream>
#include <iostream>
#include <stdexcept>

Application::Application()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    window = glfwCreateWindow(mode->width, mode->height, "Game window", glfwGetPrimaryMonitor(), nullptr);

    if (!window)
        throw std::runtime_error("Could not create window");

    width = mode->width;
    height = mode->height;

    glfwMakeContextCurrent(window);
    glfwSetWindowUserPointer(window, (void*)this);
    BindCallbacks();
    glewInit();

    playerCam = std::make_shared<Camera>(mode->width, mode->height, glm::vec3{ 0, 0, 1.f });
    // basicShader = std::make_shared<Shader>("BasicShader.vert", "BasicShader.frag");
    mainShader = std::make_shared<Shader>("Shaders/Main.vert", "Shaders/Main.frag");
    skyboxShader = std::make_shared<Shader>("Shaders/Skybox.vert", "Shaders/Skybox.frag");
    //flag = std::make_shared<Mesh>(
    //    {//verticles
    //        //stalp
    //      { /* pos */ { 1058, 331, 51 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1059, 331, 51 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1059, 332, 51 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 332, 51 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 1 }, /* text */ { 0, 0 } },

    //      { /* pos */ { 1058, 331, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1059, 331, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1059, 332, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 332, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 1 }, /* text */ { 0, 0 } },
    //      //drapel
    //      { /* pos */ { 1058, 331, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 0, 0, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 331, 54 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 0, 0, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 332, 54 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 0, 0, 1 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 332, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 0, 0, 1 }, /* text */ { 0, 0 } },

    //      { /* pos */ { 1058, 332, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 0 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 332, 54 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 0 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 333, 54 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 0 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 333, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 1, 0 }, /* text */ { 0, 0 } },

    //      { /* pos */ { 1058, 333, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 0, 0 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 333, 54 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 0, 0 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 334, 54 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 0, 0 }, /* text */ { 0, 0 } },
    //      { /* pos */ { 1058, 334, 55 }, /* normala */ { 0, 1, 0 }, /* culoare */ { 1, 0, 0 }, /* text */ { 0, 0 } }
    //    } 
    //    ,
    //    {//indices
    //        1,2,6,1,6,5,2,3,6,6,7,3,3,4,7,7,8,4,1,4,8,5,8,1,9,10,11,9,11,12,13,14,15,13,15,16,17,18,19,17,19,20
    //    }
    //);
    //constructie flag
    HeightMap hmap;
    hmap.LoadFrom("heightmap");

    terrain = std::make_shared<Terrain>();
    terrain->Generate(hmap);

    std::vector<std::string> faces = { "Skybox/right.jpg",  "Skybox/left.jpg",  "Skybox/top.jpg",
                                       "Skybox/bottom.jpg", "Skybox/front.jpg", "Skybox/back.jpg" };

    skybox = std::make_shared<CubeMap>();
    skybox->Load(faces);
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
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    util::GlEnable(GL_CULL_FACE, GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    deltaTime = 0;
    lastFrame = 0;
    totalTime = 0;

    playerCam->camSpeed = 50.f;
    playerCam->position = { 10.f, 0.f, 10.f };

    // DiffuseLight testLight{ {500.f, 300.f, 500.f}, {1.f, 1.f, 1.f} };
    DirectionalLight sunLight{ { 0.f, -1.f, 0.f }, { 1.f, 1.f, 1.f } };

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glClear(GL_DEPTH_BUFFER_BIT);

        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        totalTime += deltaTime;

        auto initialPos = playerCam->GetPosition();
        ProcessKeyboardInput();

        auto& playerPos = playerCam->position;
        float terrainHeight = terrain->HeightAt(playerPos.x, playerPos.z);

        if (initialPos.y < terrainHeight)
            initialPos.y = terrainHeight + 2.f;

        if (playerPos.y < terrainHeight)
            playerPos.y = initialPos.y;

        /*basicShader->Use();
        basicShader->SetVec3("lightPosition", testLight.position);
        basicShader->SetVec3("lightColor", testLight.color);
        basicShader->SetVec3("objectColor", { 1.f, 1.0f, 1.f });

        basicShader->SetMat4("model", glm::mat4(1));
        basicShader->SetMat4("view", playerCam->GetViewMatrix());
        basicShader->SetMat4("projection", playerCam->GetProjectionMatrix());*/

        mainShader->Use();
        mainShader->SetVec3("lightDirection", sunLight.direction);
        mainShader->SetVec3("lightColor", sunLight.color);

        mainShader->SetMat4("model", glm::mat4(1));
        mainShader->SetMat4("view", playerCam->GetViewMatrix());
        mainShader->SetMat4("projection", playerCam->GetProjectionMatrix());

        terrain->Draw(mainShader, playerCam->GetPosition());
        //aiic render ,normale 0 1 0
        
        skybox->Draw(playerCam, skyboxShader);

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

void Application::OnMouseMove(float x, float y) { playerCam->Look(x, y); }

void Application::OnScroll(double x, double y) { playerCam->Zoom(y); }

void Application::ProcessKeyboardInput()
{
    static std::unordered_map<unsigned int, Camera::MoveDirection> keyToDirection{
        { GLFW_KEY_W, Camera::MoveDirection::Forward }, { GLFW_KEY_S, Camera::MoveDirection::Backward },
        { GLFW_KEY_A, Camera::MoveDirection::Left },    { GLFW_KEY_D, Camera::MoveDirection::Right },
        { GLFW_KEY_SPACE, Camera::MoveDirection::Up },  { GLFW_KEY_LEFT_SHIFT, Camera::MoveDirection::Down }
    };

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    for (const auto& [key, direction] : keyToDirection)
    {
        if (glfwGetKey(window, key) == GLFW_PRESS)
            playerCam->Move(direction, deltaTime);
    }
}
