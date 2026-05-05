#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <algorithm>
#include <cmath>

Camera::Camera(float distance, float yaw, float pitch)
    : m_distance(distance), m_yaw(yaw), m_pitch(pitch)
{
    updatePosition();
}

void Camera::updatePosition() {
    const float p = glm::radians(m_pitch);
    const float y = glm::radians(m_yaw);
    m_pos = m_target + glm::vec3(
        m_distance * std::cos(p) * std::cos(y),
        m_distance * std::sin(p),
        m_distance * std::cos(p) * std::sin(y)
    );
}

void Camera::processInput(GLFWwindow* w, float dt) {
    if (!glfwGetWindowAttrib(w, GLFW_FOCUSED)) return;

    const float rotSpeed = 60.0f * dt;

    if (glfwGetKey(w, GLFW_KEY_LEFT)  == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_A) == GLFW_PRESS) m_yaw   -= rotSpeed;
    if (glfwGetKey(w, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_D) == GLFW_PRESS) m_yaw   += rotSpeed;
    if (glfwGetKey(w, GLFW_KEY_UP)    == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_W) == GLFW_PRESS) m_pitch += rotSpeed;
    if (glfwGetKey(w, GLFW_KEY_DOWN)  == GLFW_PRESS || glfwGetKey(w, GLFW_KEY_S) == GLFW_PRESS) m_pitch -= rotSpeed;

    const float zoomSpeed = 10.0f * dt;
    if (glfwGetKey(w, GLFW_KEY_Q) == GLFW_PRESS) m_distance = std::max(2.0f,  m_distance - zoomSpeed);
    if (glfwGetKey(w, GLFW_KEY_E) == GLFW_PRESS) m_distance = std::min(50.0f, m_distance + zoomSpeed);

    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);
    updatePosition();
}

glm::mat4 Camera::viewMatrix() const {
    return glm::lookAt(m_pos, m_target, glm::vec3(0.0f, 1.0f, 0.0f));
}
