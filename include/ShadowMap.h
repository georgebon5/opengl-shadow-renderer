#pragma once
#define GL_SILENCE_DEPRECATION
#include <glad/glad.h>

class ShadowMap {
public:
    explicit ShadowMap(int resolution = 1024);
    ~ShadowMap();
    ShadowMap(ShadowMap&&) noexcept;
    ShadowMap& operator=(ShadowMap&&) noexcept;
    ShadowMap(const ShadowMap&)            = delete;
    ShadowMap& operator=(const ShadowMap&) = delete;

    void beginPass()                        const; // bind FBO, set viewport
    void endPass(int winW, int winH)        const; // restore default FBO + viewport
    void bindTexture(int unit = 1)          const; // bind depth cubemap for sampling

    int resolution() const { return m_res; }

private:
    GLuint m_fbo     = 0;
    GLuint m_cubemap = 0;
    int    m_res;
};
