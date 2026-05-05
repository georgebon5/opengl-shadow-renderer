#pragma once
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <initializer_list>
#include <cstddef>

class Mesh {
public:
    // attribs: component counts per attribute, e.g. {3,3,2} for pos+normal+uv
    Mesh(const float* data, std::size_t byteSize, std::initializer_list<int> attribs);
    ~Mesh();
    Mesh(Mesh&&) noexcept;
    Mesh& operator=(Mesh&&) noexcept;
    Mesh(const Mesh&)            = delete;
    Mesh& operator=(const Mesh&) = delete;

    void draw(GLenum mode = GL_TRIANGLES) const;

private:
    GLuint m_vao = 0, m_vbo = 0;
    int    m_vertexCount = 0;
};
