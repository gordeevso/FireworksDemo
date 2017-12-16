#include <exception>
#include <stdexcept>

#include "Texture.hpp"

Texture::Texture()
        : mInternalFormat(GL_RGB),
          mImageFormat(GL_RGB),
          mWrapS(GL_REPEAT),
          mWrapT(GL_REPEAT),
          mFilterMin(GL_LINEAR),
          mFilterMax(GL_LINEAR)
{}

Texture::~Texture() {
    glDeleteTextures(1, &mId);
}

void Texture::Generate(GLint width,
                       GLint height,
                       uint8_t* data)
{
    if(width <= 0 || height <= 0) {
        throw std::logic_error("Parameters must be positive");
    }

    if(!data) {
        throw std::runtime_error("No image in data handled");
    }

    mWidth = width;
    mHeight = height;
    mAspectRatio = static_cast<GLfloat>(width)/height;

    glGenTextures(1, &mId);

    glBindTexture(GL_TEXTURE_2D, mId);
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 mInternalFormat,
                 width,
                 height,
                 0,
                 mImageFormat,
                 GL_UNSIGNED_BYTE,
                 data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mWrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mWrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mFilterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mFilterMax);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Bind() const noexcept {
    glBindTexture(GL_TEXTURE_2D, mId);
}

void Texture::SetInternalFormat(GLuint internalFormat) noexcept {
    mInternalFormat = internalFormat;
}

void Texture::SetImageFormat(GLuint imageFormat) noexcept {
    mImageFormat = imageFormat;
}

GLuint Texture::GetInternalFormat() const noexcept {
    return mInternalFormat;
}

GLuint Texture::GetImageFormat() const noexcept {
    return mImageFormat;
}

GLuint Texture::GetId() const noexcept {
    return mId;
}

float Texture::GetWidth() const noexcept {
    return mWidth;
}

float Texture::GetHeight() const noexcept {
    return mHeight;
}


