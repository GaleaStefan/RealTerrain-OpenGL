#pragma once
#include <GL/glew.h>
#include <glm.hpp>

class Camera
{
private:
    constexpr static float DEF_Z_NEAR = .1f;
    constexpr static float DEF_Z_FAR = 800.f;
    constexpr static float DEF_YAW = -90.f;
    constexpr static float DEF_PITCH = 0.f;
    constexpr static float DEF_FOV = 45.f;
    constexpr static float MIN_FOV = 30.f;
    constexpr static float MAX_FOV = 120.f;
    constexpr static float DEF_CAM_SPEED = 2.f;
    constexpr static float DEF_SENSIVITY = .1f;

public:
    enum class MoveDirection
    {
        Unknown,
        Forward,
        Backward,
        Left,
        Right,
        Up,
        Down
    };

public:
    Camera(int screenWidth, int screenHeight, const glm::vec3 pos) { Initialize(screenWidth, screenHeight, pos); }

    void Reshape(int screenWidth, int screenHeight);
    void Move(MoveDirection direction, float deltaTime);
    void Look(float x, float y);
    void Zoom(float value);

    const glm::mat4 GetViewMatrix() const;
    const glm::mat4 GetProjectionMatrix() const;
    const glm::vec3 GetPosition() const noexcept;

private:
    void Initialize(int screenWidth, int screenHeight, const glm::vec3 pos);
    void RecalculateLocalVectors();
    void Rotate(float xOffset, float yOffset);

public:
    float zNear = DEF_Z_NEAR;
    float zFar = DEF_Z_FAR;
    float yaw = DEF_YAW;
    float pitch = DEF_PITCH;
    float fov = DEF_FOV;
    float camSpeed = DEF_CAM_SPEED;
    float sensivity = DEF_SENSIVITY;

    float lastX;
    float lastY;

    glm::vec3 position;

private:
    bool firstMouseMove = true;

    int screenWidth;
    int screenHeight;

    glm::vec3 forward;
    glm::vec3 right;
    glm::vec3 up;
    glm::vec3 worldUp;
};
