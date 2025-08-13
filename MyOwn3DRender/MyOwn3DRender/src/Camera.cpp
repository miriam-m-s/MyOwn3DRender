#include"Camera.h"
#include <GLFW/glfw3.h>

Camera::Camera( GLFWwindow* window):clicked(false)
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
    if (cam->fov < 1.0f)
        cam->fov = 1.0f;
    if (cam->fov > 45.0f)
        cam->fov = 45.0f;
}
void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
void Camera::mouse_callback(GLFWwindow * window, double xpos, double ypos)
{
    Camera* cam = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (!cam) return;

    // Si no está haciendo clic, reiniciamos para evitar saltos
    if (!cam->clicked) {
        cam->firstMouse = true;
        return;
    }

    if (cam->firstMouse)
    {
        cam->lastX = xpos;
        cam->lastY = ypos;
        cam->firstMouse = false;
    }

    float offsetX =  cam->lastX- xpos;
    float offsetY =  ypos- cam->lastY; // invertido para movimiento natural
    cam->lastX = xpos;
    cam->lastY = ypos;

    const float sensitivity = 0.1f;
    offsetX *= sensitivity;
    offsetY *= sensitivity;

    cam->yaw += offsetX;
    cam->pitch += offsetY;

    if (cam->pitch > 89.0f)
        cam->pitch = 89.0f;
    if (cam->pitch < -89.0f)
        cam->pitch = -89.0f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
    direction.y = sin(glm::radians(cam->pitch));
    direction.z = sin(glm::radians(cam->yaw)) * cos(glm::radians(cam->pitch));
    cam->cameraFront = glm::normalize(direction);

}

void Camera::processInput(GLFWwindow* window)
{
    const float cameraSpeed = 0.05f; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        clicked = true;
    }
    else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        clicked = false;
    }
}

glm::vec3 Camera::getPosition() const
{
    return cameraPos;
}

glm::vec3 Camera::getFront() const
{
    return cameraFront;
}

glm::vec3 Camera::getUp()
{
    return cameraUp;
}

float Camera::getFov()
{
    return fov;
}

void Camera::setClicked(bool click)
{
    clicked = click;
}
