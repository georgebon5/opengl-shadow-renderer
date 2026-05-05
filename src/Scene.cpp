#include "Scene.h"
#include "../include/tiny_obj_loader.h"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <string>

// pos(3) + normal(3) + uv(2)
static constexpr float kCubeVerts[] = {
    // back face  (normal  0, 0,-1)
    -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f,0.0f,
     0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f,0.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f,1.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f,1.0f,
    -0.5f, 0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f,1.0f,
    -0.5f,-0.5f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f,0.0f,
    // front face (normal  0, 0, 1)
    -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f,0.0f,
     0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f,0.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f,1.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  1.0f,1.0f,
    -0.5f, 0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f,1.0f,
    -0.5f,-0.5f, 0.5f,  0.0f, 0.0f, 1.0f,  0.0f,0.0f,
    // left face  (normal -1, 0, 0)
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  1.0f,0.0f,
    -0.5f, 0.5f,-0.5f, -1.0f, 0.0f, 0.0f,  1.0f,1.0f,
    -0.5f,-0.5f,-0.5f, -1.0f, 0.0f, 0.0f,  0.0f,1.0f,
    -0.5f,-0.5f,-0.5f, -1.0f, 0.0f, 0.0f,  0.0f,1.0f,
    -0.5f,-0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  0.0f,0.0f,
    -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,  1.0f,0.0f,
    // right face (normal  1, 0, 0)
     0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  1.0f,0.0f,
     0.5f, 0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  1.0f,1.0f,
     0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  0.0f,1.0f,
     0.5f,-0.5f,-0.5f,  1.0f, 0.0f, 0.0f,  0.0f,1.0f,
     0.5f,-0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  0.0f,0.0f,
     0.5f, 0.5f, 0.5f,  1.0f, 0.0f, 0.0f,  1.0f,0.0f,
    // bottom face (normal  0,-1, 0)
    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,  0.0f,1.0f,
     0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,  1.0f,1.0f,
     0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,  1.0f,0.0f,
     0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,  1.0f,0.0f,
    -0.5f,-0.5f, 0.5f,  0.0f,-1.0f, 0.0f,  0.0f,0.0f,
    -0.5f,-0.5f,-0.5f,  0.0f,-1.0f, 0.0f,  0.0f,1.0f,
    // top face   (normal  0, 1, 0)
    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f,1.0f,
     0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  1.0f,1.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,
     0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  1.0f,0.0f,
    -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f,  0.0f,0.0f,
    -0.5f, 0.5f,-0.5f,  0.0f, 1.0f, 0.0f,  0.0f,1.0f,
};

static Mesh loadPlanetMesh(const char* path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    std::string base(path);
    std::size_t slash = base.find_last_of("/\\");
    base = (slash == std::string::npos) ? "" : base.substr(0, slash + 1);

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path, base.c_str())) {
        if (!warn.empty()) std::cerr << "tinyobj warn: " << warn << '\n';
        if (!err.empty())  std::cerr << "tinyobj err: "  << err  << '\n';
        throw std::runtime_error("Failed to load planet OBJ");
    }

    std::vector<float> verts;
    verts.reserve(100'000);
    for (const auto& shape : shapes) {
        std::size_t offset = 0;
        for (int fv : shape.mesh.num_face_vertices) {
            if (fv == 3) {
                for (int v = 0; v < 3; ++v) {
                    auto idx = shape.mesh.indices[offset + v];
                    verts.push_back(attrib.vertices[3 * idx.vertex_index + 0]);
                    verts.push_back(attrib.vertices[3 * idx.vertex_index + 1]);
                    verts.push_back(attrib.vertices[3 * idx.vertex_index + 2]);
                }
            }
            offset += fv;
        }
    }

    if (verts.empty()) throw std::runtime_error("Planet OBJ has no triangles");
    return Mesh(verts.data(), verts.size() * sizeof(float), {3});
}

Scene::Scene()
    : m_cubeShader  ("../shaders/basic.vs",  "../shaders/basic.fs")
    , m_planetShader("../shaders/planet.vs", "../shaders/planet.fs")
    , m_shadowShader("../shaders/shadow.vs", "../shaders/shadow.fs", "../shaders/shadow.gs")
    , m_cubeMesh    (kCubeVerts, sizeof(kCubeVerts), {3, 3, 2})
    , m_planetMesh  (loadPlanetMesh("../assets/planet/planet.obj"))
    , m_cubeTexture ("../textures/container.png")
    , m_shadowMap   (1024)
{
    m_cubeShader.use();
    m_cubeShader.set("texture1",  0);
    m_cubeShader.set("shadowMap", 1);
    m_cubeShader.set("farPlane",  FAR_PLANE);
}

// -----------------------------------------------------------------------

glm::mat4 Scene::cubeModel(int i) const {
    float angle = m_time + i * glm::radians(60.0f);
    glm::mat4 m = glm::translate(glm::mat4(1.0f), m_planetPos);
    m = glm::rotate(m, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    m = glm::translate(m, glm::vec3(PLANET_RADIUS + 1.5f, 0.0f, 0.0f));
    m = glm::rotate(m, m_time * (0.5f + i * 0.2f), glm::vec3(1.0f, 0.3f, 0.5f));
    return m;
}

std::array<glm::mat4, 6> Scene::shadowMatrices() const {
    const auto proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, FAR_PLANE);
    const auto& p   = m_planetPos;
    return {{
        proj * glm::lookAt(p, p + glm::vec3( 1, 0, 0), glm::vec3(0,-1, 0)),
        proj * glm::lookAt(p, p + glm::vec3(-1, 0, 0), glm::vec3(0,-1, 0)),
        proj * glm::lookAt(p, p + glm::vec3( 0, 1, 0), glm::vec3(0, 0, 1)),
        proj * glm::lookAt(p, p + glm::vec3( 0,-1, 0), glm::vec3(0, 0,-1)),
        proj * glm::lookAt(p, p + glm::vec3( 0, 0, 1), glm::vec3(0,-1, 0)),
        proj * glm::lookAt(p, p + glm::vec3( 0, 0,-1), glm::vec3(0,-1, 0)),
    }};
}

// -----------------------------------------------------------------------

void Scene::update(float dt) {
    if (!m_paused) m_time += dt;
    m_planetPos = {
        std::cos(m_time * PLANET_SPEED) * PLANET_RADIUS,
        0.0f,
        std::sin(m_time * PLANET_SPEED) * PLANET_RADIUS
    };
}

void Scene::draw(const Camera& camera, int winW, int winH) const {
    // ── shadow pass ──────────────────────────────────────────────────────
    // Cull front faces to reduce shadow acne on curved surfaces.
    glCullFace(GL_FRONT);

    m_shadowMap.beginPass();

    const auto mats = shadowMatrices();
    m_shadowShader.use();
    for (int i = 0; i < 6; ++i)
        m_shadowShader.set(("shadowMatrices[" + std::to_string(i) + "]").c_str(), mats[i]);
    m_shadowShader.set("lightPos",  m_planetPos);
    m_shadowShader.set("farPlane",  FAR_PLANE);

    for (int i = 0; i < NUM_CUBES; ++i) {
        m_shadowShader.set("model", cubeModel(i));
        m_cubeMesh.draw();
    }

    m_shadowMap.endPass(winW, winH);
    glCullFace(GL_BACK);

    // ── lighting pass ─────────────────────────────────────────────────────
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    const glm::mat4 view = camera.viewMatrix();
    const glm::mat4 proj = glm::perspective(
        glm::radians(45.0f), static_cast<float>(winW) / winH, 0.1f, 100.0f);

    // planet
    m_planetShader.use();
    glm::mat4 planetModel = glm::translate(glm::mat4(1.0f), m_planetPos);
    planetModel = glm::scale(planetModel, glm::vec3(0.18f));
    m_planetShader.set("model",      planetModel);
    m_planetShader.set("view",       view);
    m_planetShader.set("projection", proj);
    m_planetMesh.draw();

    // cubes
    m_cubeShader.use();
    m_cubeShader.set("view",      view);
    m_cubeShader.set("projection", proj);
    m_cubeShader.set("lightPos",  m_planetPos);
    m_cubeShader.set("viewPos",   camera.position());
    m_cubeTexture.bind(0);
    m_shadowMap.bindTexture(1);

    for (int i = 0; i < NUM_CUBES; ++i) {
        m_cubeShader.set("model", cubeModel(i));
        m_cubeMesh.draw();
    }
}
