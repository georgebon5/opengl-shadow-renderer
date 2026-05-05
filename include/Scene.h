#pragma once
#include "Shader.h"
#include "Mesh.h"
#include "Texture.h"
#include "Camera.h"
#include "ShadowMap.h"
#include <glm/glm.hpp>
#include <array>

class Scene {
public:
    Scene();

    void update(float dt);
    void draw(const Camera& camera, int winW, int winH) const;
    void togglePause() { m_paused = !m_paused; }

private:
    Shader    m_cubeShader;
    Shader    m_planetShader;
    Shader    m_shadowShader;
    Mesh      m_cubeMesh;
    Mesh      m_planetMesh;
    Texture   m_cubeTexture;
    ShadowMap m_shadowMap;

    float     m_time      = 0.0f;
    bool      m_paused    = false;
    glm::vec3 m_planetPos = {};

    static constexpr float PLANET_SPEED  = 0.5f;
    static constexpr float PLANET_RADIUS = 3.0f;
    static constexpr float FAR_PLANE     = 25.0f;
    static constexpr int   NUM_CUBES     = 6;

    glm::mat4 cubeModel(int i) const;
    std::array<glm::mat4, 6> shadowMatrices() const;
};
