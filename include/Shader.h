#pragma once
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class Shader {
public:
    Shader(const char* vertPath, const char* fragPath, const char* geomPath = nullptr);
    ~Shader();
    Shader(Shader&&) noexcept;
    Shader& operator=(Shader&&) noexcept;
    Shader(const Shader&)            = delete;
    Shader& operator=(const Shader&) = delete;

    void use() const;
    void set(const char* name, int v)              const;
    void set(const char* name, float v)            const;
    void set(const char* name, const glm::vec3& v) const;
    void set(const char* name, const glm::mat4& v) const;

private:
    GLuint m_id = 0;
    static GLuint      compile(GLenum type, const std::string& src);
    static std::string load(const char* path);
};
