#include "Shader.h"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdexcept>

std::string Shader::load(const char* path) {
    std::ifstream f(path);
    if (!f) throw std::runtime_error(std::string("Cannot open shader: ") + path);
    std::stringstream ss;
    ss << f.rdbuf();
    return ss.str();
}

GLuint Shader::compile(GLenum type, const std::string& src) {
    GLuint id = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(id, 1, &c, nullptr);
    glCompileShader(id);
    int ok = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetShaderInfoLog(id, 1024, nullptr, log);
        std::cerr << "Shader compile error:\n" << log << '\n';
    }
    return id;
}

Shader::Shader(const char* vertPath, const char* fragPath, const char* geomPath) {
    GLuint vs = compile(GL_VERTEX_SHADER,   load(vertPath));
    GLuint fs = compile(GL_FRAGMENT_SHADER, load(fragPath));
    m_id = glCreateProgram();
    glAttachShader(m_id, vs);
    glAttachShader(m_id, fs);
    if (geomPath) {
        GLuint gs = compile(GL_GEOMETRY_SHADER, load(geomPath));
        glAttachShader(m_id, gs);
        glDeleteShader(gs);
    }
    glLinkProgram(m_id);
    int ok = 0;
    glGetProgramiv(m_id, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[1024];
        glGetProgramInfoLog(m_id, 1024, nullptr, log);
        std::cerr << "Program link error:\n" << log << '\n';
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader()                     { if (m_id) glDeleteProgram(m_id); }
Shader::Shader(Shader&& o) noexcept   : m_id(o.m_id) { o.m_id = 0; }
Shader& Shader::operator=(Shader&& o) noexcept {
    if (this != &o) { if (m_id) glDeleteProgram(m_id); m_id = o.m_id; o.m_id = 0; }
    return *this;
}

void Shader::use() const { glUseProgram(m_id); }

void Shader::set(const char* n, int v)              const { glUniform1i (glGetUniformLocation(m_id, n), v); }
void Shader::set(const char* n, float v)            const { glUniform1f (glGetUniformLocation(m_id, n), v); }
void Shader::set(const char* n, const glm::vec3& v) const { glUniform3fv(glGetUniformLocation(m_id, n), 1, glm::value_ptr(v)); }
void Shader::set(const char* n, const glm::mat4& v) const { glUniformMatrix4fv(glGetUniformLocation(m_id, n), 1, GL_FALSE, glm::value_ptr(v)); }
