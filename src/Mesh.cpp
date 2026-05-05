#include "Mesh.h"

Mesh::Mesh(const float* data, std::size_t byteSize, std::initializer_list<int> attribs) {
    int stride = 0;
    for (int a : attribs) stride += a;
    m_vertexCount = static_cast<int>(byteSize / (stride * sizeof(float)));

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glBindVertexArray(m_vao);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(byteSize), data, GL_STATIC_DRAW);

    int offset = 0, idx = 0;
    for (int size : attribs) {
        glVertexAttribPointer(
            idx, size, GL_FLOAT, GL_FALSE,
            stride * static_cast<int>(sizeof(float)),
            reinterpret_cast<void*>(static_cast<std::ptrdiff_t>(offset) * sizeof(float)));
        glEnableVertexAttribArray(idx);
        offset += size;
        ++idx;
    }
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    if (m_vbo) glDeleteBuffers(1, &m_vbo);
    if (m_vao) glDeleteVertexArrays(1, &m_vao);
}

Mesh::Mesh(Mesh&& o) noexcept
    : m_vao(o.m_vao), m_vbo(o.m_vbo), m_vertexCount(o.m_vertexCount)
{
    o.m_vao = o.m_vbo = 0;
    o.m_vertexCount   = 0;
}

Mesh& Mesh::operator=(Mesh&& o) noexcept {
    if (this != &o) {
        if (m_vbo) glDeleteBuffers(1, &m_vbo);
        if (m_vao) glDeleteVertexArrays(1, &m_vao);
        m_vao = o.m_vao; m_vbo = o.m_vbo; m_vertexCount = o.m_vertexCount;
        o.m_vao = o.m_vbo = 0; o.m_vertexCount = 0;
    }
    return *this;
}

void Mesh::draw(GLenum mode) const {
    glBindVertexArray(m_vao);
    glDrawArrays(mode, 0, m_vertexCount);
    glBindVertexArray(0);
}
