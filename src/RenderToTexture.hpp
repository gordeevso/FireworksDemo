#pragma once

#include <memory>

#include "Shader.hpp"

class RenderToTexture {
public:
    RenderToTexture();
    ~RenderToTexture();
    RenderToTexture(RenderToTexture const &) = default;
    RenderToTexture &operator=(RenderToTexture const &) = default;

    void Init();
    void PreRender();
    void PostRender();

private:
    std::shared_ptr<Shader> mShader;
    GLuint mFBO;
    GLuint mTBO;
    GLuint mRBO;
    GLuint mVAO;
    GLuint mVBO;
};
