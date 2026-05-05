#pragma once
#define GL_SILENCE_DEPRECATION
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// Orbital camera — always looks at the world origin.
// LEFT/RIGHT arrows orbit around the Y axis.
// UP/DOWN arrows orbit around the X axis.
class Camera {
public:
    Camera(float distance, float yaw, float pitch);

    void      processInput(GLFWwindow* window, float dt);
    glm::mat4 viewMatrix() const;
    glm::vec3 position()   const { return m_pos; }

private:
    glm::vec3 m_target   = glm::vec3(0.0f);
    float     m_distance = 12.0f;
    float     m_yaw      = 0.0f;
    float     m_pitch    = 20.0f;
    glm::vec3 m_pos      = {};

    void updatePosition();
};
