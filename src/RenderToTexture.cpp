#include <iostream>

#include "RenderToTexture.hpp"
#include "ResourceManager.hpp"
#include "GLFWWrapper.hpp"


RenderToTexture::RenderToTexture() : mShader{},
                                     mFBO{},
                                     mTBO{},
                                     mRBO{},
                                     mVAO{},
                                     mVBO{} {
    RenderToTexture::Init();
}

RenderToTexture::~RenderToTexture() {
    glDeleteTextures(1, &mTBO);
    glDeleteFramebuffers(1, &mFBO);
    glDeleteRenderbuffers(1, &mRBO);
    glDeleteVertexArrays(1, &mVAO);
    glDeleteBuffers(1, &mVBO);
}

void RenderToTexture::Init() {
    GLfloat quadVertices[] = {
            // positions   // texCoords
            -1.0f, 1.0f,  0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f,  1.0f, 0.0f,

            -1.0f, 1.0f,  0.0f, 1.0f,
            1.0f, -1.0f,  1.0f, 0.0f,
            1.0f,  1.0f,  1.0f, 1.0f
    };
    glGenVertexArrays(1, &mVAO);
    glGenBuffers(1, &mVBO);

    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    glBindVertexArray(mVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(2 * sizeof(GLfloat)));

    mShader = ResourceManager::GetShader(RENDER_TO_TEXTURE_PROGRAM);
    mShader->LinkProgram();
    mShader->Use();
    mShader->SetUniform("screenTexture", 0);

    int32_t screenWidth = GLFWWrapper::GetInstance()->GetWidth();
    int32_t screenHeight = GLFWWrapper::GetInstance()->GetHeight();

    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glGenTextures(1, &mTBO);
    glBindTexture(GL_TEXTURE_2D, mTBO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTBO, 0);

//    glGenRenderbuffers(1, &mRBO);
//    glBindRenderbuffer(GL_RENDERBUFFER, mRBO);
//    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
//    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mRBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!\n";
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderToTexture::PreRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    glClearColor(0.2f, 0.7f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void RenderToTexture::PostRender() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
    glClear(GL_COLOR_BUFFER_BIT);

    mShader->Use();
    glBindVertexArray(mVAO);
    glBindTexture(GL_TEXTURE_2D, mTBO);	// use the color attachment texture as the texture of the quad plane
    glDrawArrays(GL_TRIANGLES, 0, 6);
}
