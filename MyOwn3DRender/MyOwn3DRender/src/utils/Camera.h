#pragma once
#include <glm/glm.hpp>
struct GLFWwindow;
class Camera {
public:
	Camera(struct GLFWwindow* window);
	
	void updateCameraVectors();
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void processInput(GLFWwindow* window);
	glm::vec3 getPosition() const;
	glm::vec3 getFront() const;
	glm::vec3 getUp();
	float getFov();
	void setClicked(bool click);
	// Setters
	void setPosition(const glm::vec3& pos) { cameraPos = pos; }
	void setFront(const glm::vec3& front) { cameraFront = glm::normalize(front); }
	void setUp(const glm::vec3& up) { cameraUp = glm::normalize(up); }
private:
	
	
	glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
	double yaw = -90;
	double pitch = 0;
	bool firstMouse = true;
	bool clicked = false;
	float fov = 45;

	float lastX = 400, lastY = 300;
};