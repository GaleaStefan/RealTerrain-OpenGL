#include "Camera.h"
#include <gtc/matrix_transform.hpp>
#include <unordered_map>

const glm::mat4 Camera::GetProjectionMatrix() const
{
    glm::mat4 projMat = glm::mat4(1);
    float aspectRatio = (float)screenWidth / screenHeight;
    projMat = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);
    return projMat;
}

const glm::mat4 Camera::GetViewMatrix() const { return glm::lookAt(position, position + forward, up); }

const glm::vec3 Camera::GetPosition() const noexcept { return position; }

void Camera::Look(float x, float y)
{
    if (firstMouseMove)
    {
        lastX = x;
        lastY = y;
        firstMouseMove = false;
        return;
    }

    float xOffset = x - lastX;
    float yOffset = lastY - y;
    lastX = x;
    lastY = y;

    constexpr static float MIN_OFFSET = 1e-6;

    if (glm::abs(xOffset) <= MIN_OFFSET && glm::abs(yOffset) <= MIN_OFFSET)
        return;

    Rotate(xOffset * sensivity, yOffset * sensivity);
}

void Camera::Zoom(float value) { fov = glm::clamp(fov + value, MIN_FOV, MAX_FOV); }

void Camera::Move(MoveDirection direction, float deltaTime)
{
    static std::unordered_map<MoveDirection, std::pair<glm::vec3 Camera::*, float>> moveVector{
        { MoveDirection::Forward, { &Camera::forward, 1.f } }, { MoveDirection::Backward, { &Camera::forward, -1.f } },
        { MoveDirection::Left, { &Camera::right, -1.f } },     { MoveDirection::Right, { &Camera::right, 1.f } },
        { MoveDirection::Up, { &Camera::up, 1.f } },           { MoveDirection::Down, { &Camera::up, -1.f } },
    };

    float velocity = camSpeed * deltaTime;
    auto& [vectorPointer, sign] = moveVector[direction];
    position = position + this->*vectorPointer * velocity * sign;
}

void Camera::Initialize(int screenWidth, int screenHeight, const glm::vec3 pos)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;

    lastX = screenWidth / 2.f;
    lastY = screenHeight / 2.f;

    firstMouseMove = true;

    position = pos;
    worldUp = glm::vec3(0.f, 1.f, 0.f);

    RecalculateLocalVectors();
}

void Camera::RecalculateLocalVectors()
{
    float yawRadians = glm::radians(yaw);
    float pitchRadians = glm::radians(pitch);

    forward = glm::vec3{ glm::cos(yawRadians) * glm::cos(pitchRadians), glm::sin(pitchRadians),
                         glm::sin(yawRadians) * glm::cos(pitchRadians) };

    forward = glm::normalize(forward);

    right = glm::normalize(glm::cross(forward, worldUp));
    up = glm::normalize(glm::cross(right, forward));
}

void Camera::Rotate(float xOffset, float yOffset)
{
    yaw += xOffset;
    pitch += yOffset;

    pitch = glm::clamp(pitch, -89.f, 89.f);
    RecalculateLocalVectors();
}

void Camera::Reshape(int windowWidth, int windowHeight)
{
    screenWidth = windowWidth;
    screenHeight = windowHeight;

    glViewport(0, 0, windowWidth, windowHeight);
}
