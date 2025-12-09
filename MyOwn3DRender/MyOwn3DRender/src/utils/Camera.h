#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;

class Camera {
public:
    Camera(GLFWwindow* window);

    void updateCameraVectors();
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos);

    void processInput(GLFWwindow* window);

    glm::vec3 getPosition() const;
    glm::vec3 getFront() const;
    glm::vec3 getUp() const;
    float getFov() const;

    void setClicked(bool click);

    // Setters
    void setPosition(const glm::vec3& pos) { cameraPos = pos; }
    void setFront(const glm::vec3& front) { cameraFront = glm::normalize(front); }
    void setUp(const glm::vec3& up) { cameraUp = glm::normalize(up); }

private:
    // Variables de cámara
    glm::vec3 cameraPos{ 0.0f, 0.0f, 3.0f };
    glm::vec3 cameraFront{ 0.0f, 0.0f, -1.0f };
    glm::vec3 cameraUp{ 0.0f, 1.0f, 0.0f };

    // Movimiento suave FPS style
    glm::vec3 velocity{ 0.0f };

    float yaw{ -90.0f };
    float pitch{ 0.0f };

    float fov{ 45.0f };

    bool clicked{ false };
    bool firstMouse{ true };

    float lastX{ 400.0f };
    float lastY{ 300.0f };

    float deltaTime{ 0.0f };
    float lastFrame{ 0.0f };
};
