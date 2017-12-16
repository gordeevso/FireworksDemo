#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "ParticleSystem.hpp"
#include "ResourceManager.hpp"
#include "GLFWWrapper.hpp"
#include "Random.hpp"

namespace Particles {


    ParticleSystem::ParticleSystem(int32_t maxParticles) : mBInitialized(false),
                                                           mIntCurParticleBuffer(0),
                                                           mIntMaxParticles(maxParticles),
                                                           mIntNumParticleAttributes(6)
    {
    }

    bool ParticleSystem::Init() {
        if (mBInitialized)return false;

        mMat4Projection = glm::ortho(0.0f,
                                     static_cast<GLfloat>(GLFWWrapper::GetInstance()->GetWidth()),
                                     0.0f,
                                     static_cast<GLfloat>(GLFWWrapper::GetInstance()->GetHeight()),
                                     -1.0f,
                                     1.0f);

        mPtrTextureParticle = ResourceManager::GetTexture(PARTICLE_TEXTURE1);

        mPtrDefaultProgramUpdate = ResourceManager::GetShader(PARTICLE_COMMON_UPDATE_PROGRAM);
        mPtrDefaultProgramUpdate->LinkProgram();

        mPtrDefaultProgramRender = ResourceManager::GetShader(PARTICLE_COMMON_RENDER_PROGRAM);
        mPtrDefaultProgramRender->LinkProgram();

        glGenTransformFeedbacks(1, &mUintTFBuffer);
        glGenQueries(1, &mUintQueryParticleNum);

        glGenBuffers(2, mUintParticleBuffer);
        glGenVertexArrays(2, mUintVAO);

        Particle partInitialization;
        partInitialization.iType = 0;

        for (uint32_t i = 0; i != 2; ++i)
        {
            glBindVertexArray(mUintVAO[i]);
            glBindBuffer(GL_ARRAY_BUFFER, mUintParticleBuffer[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(Particle) * mIntMaxParticles, nullptr, GL_DYNAMIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Particle), &partInitialization);

            for (int32_t j = 0; j != mIntNumParticleAttributes; ++j)
            {
                glEnableVertexAttribArray(j);
            }

            int32_t offset = 0;
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid *>(offset));
            offset += sizeof(Particle::vPosition);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid *>(offset));
            offset += sizeof(Particle::vVelocity);
            glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid *>(offset));
            offset += sizeof(Particle::vColor);
            glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid *>(offset));
            offset += sizeof(Particle::fLifeTime);
            glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid *>(offset));
            offset += sizeof(Particle::fSize);
            glVertexAttribPointer(5, 1, GL_INT, GL_FALSE, sizeof(Particle), reinterpret_cast<const GLvoid *>(offset));

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
        }

        mIntCurParticleBuffer = 0;
        mIntCurNumParticles = 1;

        mBInitialized = true;

        return true;
    }


    void ParticleSystem::Update(float fTimePassed) {
        if (!mBInitialized)return;

        mPtrDefaultProgramUpdate->Use();

        mPtrDefaultProgramUpdate->SetUniform("fTimePassed", fTimePassed);
        mPtrDefaultProgramUpdate->SetUniform("mVec2EmitPosition", mVec2EmitPosition);
        mPtrDefaultProgramUpdate->SetUniform("mVec2EmitVelocityMin", mVec2EmitVelocityMin);
        mPtrDefaultProgramUpdate->SetUniform("mVec2EmitVelocityRange", mVec2EmitVelocityRange);
        mPtrDefaultProgramUpdate->SetUniform("mVec3EmitColor", mVec3EmitColor);
        mPtrDefaultProgramUpdate->SetUniform("mVec2EmitGravity", mVec2EmitGravity);

        mPtrDefaultProgramUpdate->SetUniform("mFEmitLifeTimeMin", mFEmitLifeTimeMin);
        mPtrDefaultProgramUpdate->SetUniform("mFEmitLifeTimeRange", mFEmitLifeTimeRange);

        mPtrDefaultProgramUpdate->SetUniform("mFEmitSize", mFEmitSize);
        mPtrDefaultProgramUpdate->SetUniform("mIntNumTargetEmission", 0);

        mFDeltaTime += fTimePassed;

        if (mFDeltaTime > mFNextEmissionTime) {
            mPtrDefaultProgramUpdate->SetUniform("mIntNumTargetEmission", mIntNumTargetEmission);
            mFDeltaTime -= mFNextEmissionTime;

            glm::vec3 vRandomSeed;
            vRandomSeed.x = Random::getRandomZeroToOne();
            vRandomSeed.y = Random::getRandomZeroToOne();
            vRandomSeed.z = Random::getRandomZeroToOne();
            mPtrDefaultProgramUpdate->SetUniform("vRandomSeed", vRandomSeed);
        }

        glEnable(GL_RASTERIZER_DISCARD);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mUintTFBuffer);

        glBindVertexArray(mUintVAO[mIntCurParticleBuffer]);
        glEnableVertexAttribArray(1); // Re-enable velocity

        ParticleSystem::SwapParticleBuffers();
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mUintParticleBuffer[mIntCurParticleBuffer]);

        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, mUintQueryParticleNum);
        glBeginTransformFeedback(GL_POINTS);

        glDrawArrays(GL_POINTS, 0, mIntCurNumParticles);

        glEndTransformFeedback();

        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
        glGetQueryObjectiv(mUintQueryParticleNum, GL_QUERY_RESULT, &mIntCurNumParticles);

        glBindVertexArray(0);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

        glDisable(GL_RASTERIZER_DISCARD);
    }

    void ParticleSystem::Render() {
        if (!mBInitialized)return;

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        mPtrDefaultProgramRender->Use();
        mPtrDefaultProgramRender->SetUniform("umProjection", mMat4Projection);
        mPtrDefaultProgramRender->SetUniform("gSampler", 0);

        mPtrTextureParticle->Bind();

        glBindVertexArray(mUintVAO[mIntCurParticleBuffer]);
        glDisableVertexAttribArray(1);

        glDrawArrays(GL_POINTS, 0, mIntCurNumParticles);

        glDisable(GL_BLEND);

    }


    void ParticleSystem::SetEmitterParameters(glm::vec2 a_vGenPosition,
                                              glm::vec2 a_vGenVelocityMin,
                                              glm::vec2 a_vGenVelocityMax,
                                              glm::vec2 a_vGenGravityVector,
                                              glm::vec3 a_vGenColor,
                                              float a_fGenLifeMin,
                                              float a_fGenLifeMax,
                                              float a_fGenSize,
                                              float fEvery,
                                              int a_iNumToGenerate) {
        mVec2EmitPosition = a_vGenPosition;
        mVec2EmitVelocityMin = a_vGenVelocityMin;
        mVec2EmitVelocityRange = a_vGenVelocityMax - a_vGenVelocityMin;

        mVec2EmitGravity = a_vGenGravityVector;
        mVec3EmitColor = a_vGenColor;
        mFEmitSize = a_fGenSize;

        mFEmitLifeTimeMin = a_fGenLifeMin;
        mFEmitLifeTimeRange = a_fGenLifeMax - a_fGenLifeMin;

        mFNextEmissionTime = fEvery;
        mFDeltaTime = 0.8f;

        mIntNumTargetEmission = a_iNumToGenerate;
    }

    const glm::vec2 &ParticleSystem::getMVec2EmitPosition() const
    {
        return mVec2EmitPosition;
    }


    void ParticleSystem::setMVec2EmitPosition(const glm::vec2 &mVec2EmitPosition)
    {
        ParticleSystem::mVec2EmitPosition = mVec2EmitPosition;
    }
    const glm::vec2 &ParticleSystem::getMVec2EmitVelocityMin() const
    {
        return mVec2EmitVelocityMin;
    }


    const glm::vec2 &ParticleSystem::getMVec2EmitVelocityRange() const
    {
        return mVec2EmitVelocityRange;
    }
    void ParticleSystem::setMVec2EmitVelocityRange(const glm::vec2 &mVec2EmitVelocityMin, const glm::vec2 &mVec2EmitVelocityMax)
    {
        ParticleSystem::mVec2EmitVelocityMin = mVec2EmitVelocityMin;
        ParticleSystem::mVec2EmitVelocityRange = mVec2EmitVelocityMax - mVec2EmitVelocityMin;
    }


    const glm::vec2 &ParticleSystem::getMVec2EmitGravity() const
    {
        return mVec2EmitGravity;
    }
    void ParticleSystem::setMVec2EmitGravity(const glm::vec2 &mVec2EmitGravity)
    {
        ParticleSystem::mVec2EmitGravity = mVec2EmitGravity;
    }


    const glm::vec3 &ParticleSystem::getMVec3EmitColor() const
    {
        return mVec3EmitColor;
    }
    void ParticleSystem::setMVec3EmitColor(const glm::vec3 &mVec3EmitColor)
    {
        ParticleSystem::mVec3EmitColor = mVec3EmitColor;
    }


    float ParticleSystem::getMFEmitLifeTimeMin() const
    {
        return mFEmitLifeTimeMin;
    }


    float ParticleSystem::getMFEmitLifeTimeRange() const {
        return mFEmitLifeTimeRange;
    }
    void ParticleSystem::setMFEmitLifeTimeRange(float mFEmitLifeTimeMin, float mFEmitLifeTimeMax)
    {
        ParticleSystem::mFEmitLifeTimeMin = mFEmitLifeTimeMin;
        ParticleSystem::mFEmitLifeTimeRange = mFEmitLifeTimeMax - mFEmitLifeTimeMin;
    }


    float ParticleSystem::getMFEmitSize() const
    {
        return mFEmitSize;
    }
    void ParticleSystem::setMFEmitSize(float mFEmitSize)
    {
        ParticleSystem::mFEmitSize = mFEmitSize;
    }


    int32_t ParticleSystem::getMIntNumTargetEmission() const
    {
        return mIntNumTargetEmission;
    }
    void ParticleSystem::setMIntNumTargetEmission(int32_t mIntNumTargetEmission)
    {
        ParticleSystem::mIntNumTargetEmission = mIntNumTargetEmission;
    }


    int ParticleSystem::GetNumParticles() const {
        return mIntCurNumParticles;
    }

    ParticleSystem::~ParticleSystem() {
        glDeleteTransformFeedbacks(1, &mUintTFBuffer);
        glDeleteQueries(1, &mUintQueryParticleNum);

        glDeleteBuffers(2, mUintParticleBuffer);
        glDeleteVertexArrays(2, mUintVAO);
    }


}