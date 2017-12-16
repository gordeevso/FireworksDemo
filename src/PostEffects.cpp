#include <iostream>
#include "PostEffects.hpp"
#include "GLFWWrapper.hpp"
#include "ResourceManager.hpp"

PostEffects::PostEffects() {
    Init();
}

PostEffects::~PostEffects() {
    glDeleteFramebuffers(1, &mhdrFBO);
    glDeleteTextures(2, mColorBuffers);
    glDeleteFramebuffers(2, mMirrorColorBuffersFBO);
    glDeleteTextures(2, mMirrorColorBuffersFBO);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

void PostEffects::Init()
{
//    mLightProgram = ResourceManager::GetShader(EFFECTS_LIGHT_PROGRAM);
//    mLightProgram->LinkProgram();
    mBlurProgram = ResourceManager::GetShader(EFFECTS_BLUR_PROGRAM);
    mBlurProgram->LinkProgram();
    mBlurProgram->Use();
    mBlurProgram->SetUniform("image", 0);

    mResultProgram = ResourceManager::GetShader(EFFECTS_FINAL_PROGRAM);
    mResultProgram->LinkProgram();
    mResultProgram->Use();
    mResultProgram->SetUniform("scene", 0);
    mResultProgram->SetUniform("bloomblur", 1);

    float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindVertexArray(mVAO);
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    glGenFramebuffers(1, &mhdrFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mhdrFBO);
    // create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
    glGenTextures(2, mColorBuffers);
    for (uint32_t i = 0; i < 2; i++)
    {
        glBindTexture(GL_TEXTURE_2D, mColorBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GLFWWrapper::GetInstance()->GetWidth(), GLFWWrapper::GetInstance()->GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        // attach texture to framebuffer
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mColorBuffers[i], 0);
    }
    // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    mAttachments[0] = GL_COLOR_ATTACHMENT0;
    mAttachments[1] = GL_COLOR_ATTACHMENT1;
    glDrawBuffers(2, mAttachments);
    // finally check if framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        throw std::runtime_error("Framebuffer not complete!");
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ping-pong-framebuffer for blurring
    glGenFramebuffers(2, mMirrorFBO);
    glGenTextures(2, mMirrorColorBuffersFBO);
    for (uint32_t i = 0; i < 2; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mMirrorFBO[i]);
        glBindTexture(GL_TEXTURE_2D, mMirrorColorBuffersFBO[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, GLFWWrapper::GetInstance()->GetWidth(), GLFWWrapper::GetInstance()->GetHeight(), 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // we clamp to the edge as the blur filter would otherwise sample repeated texture values!
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mMirrorColorBuffersFBO[i], 0);
        // also check if framebuffers are complete (no need for depth buffer)
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            throw std::runtime_error("Framebuffer not complete!");
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }
}

void PostEffects::PreRender() {

    glClearColor(0.f, 0.f, 0.f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, mhdrFBO);
    glClear(GL_COLOR_BUFFER_BIT);
}

void PostEffects::PostRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    auto errorCode = glGetError();
    if (errorCode != GL_NO_ERROR) {
        std::cerr << "DRAW ERROR " << errorCode << "\n";
    }

    bool horizontal = true;
    bool first_iteration = true;
    unsigned int amount = 10;
    mBlurProgram->Use();
    for (uint32_t i = 0; i < amount; i++)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, mMirrorFBO[horizontal]);
        mBlurProgram->SetUniform("horizontal", horizontal);
        glBindTexture(GL_TEXTURE_2D, first_iteration ? mColorBuffers[1] : mMirrorColorBuffersFBO[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)

        glBindVertexArray(mVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);

        horizontal = !horizontal;
        if (first_iteration)
            first_iteration = false;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

    }


    glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    mResultProgram->Use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mColorBuffers[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mMirrorColorBuffersFBO[1]);
    mResultProgram->SetUniform("bloom", true);
    mResultProgram->SetUniform("exposure", 1.f);

    glBindVertexArray(mVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
