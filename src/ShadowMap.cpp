#include "ShadowMap.h"
#include <iostream>

ShadowMap::ShadowMap(int resolution) : m_res(resolution) {
    glGenTextures(1, &m_cubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
    for (int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
                     m_res, m_res, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glGenFramebuffers(1, &m_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_cubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ShadowMap: framebuffer incomplete\n";

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowMap::~ShadowMap() {
    if (m_fbo)     glDeleteFramebuffers(1, &m_fbo);
    if (m_cubemap) glDeleteTextures(1, &m_cubemap);
}

ShadowMap::ShadowMap(ShadowMap&& o) noexcept
    : m_fbo(o.m_fbo), m_cubemap(o.m_cubemap), m_res(o.m_res)
{
    o.m_fbo = o.m_cubemap = 0;
}

ShadowMap& ShadowMap::operator=(ShadowMap&& o) noexcept {
    if (this != &o) {
        if (m_fbo)     glDeleteFramebuffers(1, &m_fbo);
        if (m_cubemap) glDeleteTextures(1, &m_cubemap);
        m_fbo = o.m_fbo; m_cubemap = o.m_cubemap; m_res = o.m_res;
        o.m_fbo = o.m_cubemap = 0;
    }
    return *this;
}

void ShadowMap::beginPass() const {
    glViewport(0, 0, m_res, m_res);
    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::endPass(int winW, int winH) const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, winW, winH);
}

void ShadowMap::bindTexture(int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubemap);
}
