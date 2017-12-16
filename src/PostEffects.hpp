#pragma once

#include <memory>

#include "Shader.hpp"

class PostEffects
{
public:
    PostEffects();
    ~PostEffects();
    PostEffects(PostEffects const &) = default;
    PostEffects &operator=(PostEffects const &) = default;

    void Init();
    void PreRender();
    void PostRender();

private:
    std::shared_ptr<Shader> mLightProgram;
    std::shared_ptr<Shader> mBlurProgram;
    std::shared_ptr<Shader> mResultProgram;
    GLuint mhdrFBO = 0;
    GLuint mVAO = 0;
    GLuint mVBO = 0;
    GLuint mColorBuffers[2];
    GLuint mAttachments[2];
    GLuint mMirrorFBO[2];
    GLuint mMirrorColorBuffersFBO[2];
};

