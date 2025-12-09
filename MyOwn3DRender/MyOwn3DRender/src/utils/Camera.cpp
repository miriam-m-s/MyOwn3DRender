#include "Camera.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

Camera::Camera(GLFWwindow* window)
{
    glfwSetWindowUserPointer(window, this);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetScrollCallback(window, Camera::scroll_callback);
    glfwSetCursorPosCallback(window, Camera::mouse_callback);
}

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam) return;

    cam->fov -= (float)yoffset;
    cam->fov = glm::clamp(cam->fov, 1.0f, 45.0f);
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    glm::vec3 smoothFront = glm::mix(cameraFront, front, 0.15f);
    cameraFront = glm::normalize(smoothFront);
}

void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam) return;

    if (!cam->clicked) {
        cam->firstMouse = true;
        return;
    }

    if (cam->firstMouse) {
        cam->lastX = xpos;
        cam->lastY = ypos;
        cam->firstMouse = false;
    }

    float offsetX = xpos - cam->lastX;
    float offsetY = cam->lastY - ypos;

    cam->lastX = xpos;
    cam->lastY = ypos;

    offsetX *= 0.1f;
    offsetY *= 0.1f;

    // objetivo suave
    float targetYaw = cam->yaw + offsetX;
    float targetPitch = cam->pitch + offsetY;

    targetPitch = glm::clamp(targetPitch, -89.0f, 89.0f);

    cam->yaw = glm::mix(cam->yaw, targetYaw, 0.25f);
    cam->pitch = glm::mix(cam->pitch, targetPitch, 0.25f);

    cam->updateCameraVectors();
}

void Camera::processInput(GLFWwindow* window)
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    float speedMultiplier = clicked ? 0.5f : 0.5f;
    float cameraSpeed = speedMultiplier * deltaTime;
    glm::vec3 acceleration(0.0f);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        acceleration += cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        acceleration -= cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        acceleration -= glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        acceleration += glm::normalize(glm::cross(cameraFront, cameraUp));
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        acceleration += cameraUp;
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        acceleration -= cameraUp;
    // Movimiento suave
    velocity += acceleration * cameraSpeed;
    cameraPos += velocity;

    // frenado natural
    velocity *= 0.85f;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        clicked = true;
    else
        clicked = false;
}

glm::vec3 Camera::getPosition() const
{
    return cameraPos;
}

glm::vec3 Camera::getFront() const
{
    return cameraFront;
}

glm::vec3 Camera::getUp() const
{
    return cameraUp;
}

float Camera::getFov() const
{
    return fov;
}

void Camera::setClicked(bool click)
{
    clicked = click;
}
