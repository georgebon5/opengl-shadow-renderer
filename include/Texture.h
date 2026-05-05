#pragma once
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>

class Texture {
public:
    explicit Texture(const char* path);
    ~Texture();
    Texture(Texture&&) noexcept;
    Texture& operator=(Texture&&) noexcept;
    Texture(const Texture&)            = delete;
    Texture& operator=(const Texture&) = delete;

    void bind(int unit = 0) const;

private:
    GLuint m_id = 0;
};
