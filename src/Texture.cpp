#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const char* path) {
    glGenTextures(1, &m_id);
    glBindTexture(GL_TEXTURE_2D, m_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,     GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    int w, h, c;
    unsigned char* img = stbi_load(path, &w, &h, &c, 0);
    if (!img) {
        std::cerr << "Texture load failed: " << path
                  << " — " << stbi_failure_reason() << '\n';
        return;
    }
    GLenum fmt = (c == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, fmt, GL_UNSIGNED_BYTE, img);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(img);
}

Texture::~Texture()                     { if (m_id) glDeleteTextures(1, &m_id); }
Texture::Texture(Texture&& o) noexcept  : m_id(o.m_id) { o.m_id = 0; }
Texture& Texture::operator=(Texture&& o) noexcept {
    if (this != &o) { if (m_id) glDeleteTextures(1, &m_id); m_id = o.m_id; o.m_id = 0; }
    return *this;
}

void Texture::bind(int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, m_id);
}
