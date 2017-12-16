#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <cmath>

#include "Random.hpp"
#include "Firework.hpp"
#include "ResourceManager.hpp"
#include "GLFWWrapper.hpp"
#include "FireworksDemoEngine.hpp"

namespace Particles {

    Firework::Firework(int32_t maxParticles) : ParticleSystem(maxParticles)
    {
        Init();
    }

    bool Firework::Init()
    {
        if (mBInitialized)return false;

        mMat4Projection = glm::ortho(0.0f,
                                     static_cast<GLfloat>(GLFWWrapper::GetInstance()->GetWidth()),
                                     0.0f,
                                     static_cast<GLfloat>(GLFWWrapper::GetInstance()->GetHeight()),
                                     -1.0f,
                                     1.0f);

        mPtrTextureParticle = ResourceManager::GetTexture(PARTICLE_TEXTURE + std::to_string(Random::getRandom(1,9)));


        mPtrDefaultProgramUpdate = ResourceManager::GetShader(PARTICLE_STRAFE_UPDATE_PROGRAM);
        mPtrDefaultProgramUpdate->LinkProgram();

        mPtrOptionalProgramUpdate = ResourceManager::GetShader(PARTICLE_EXPLODE_UPDATE_PROGRAM);
        mPtrOptionalProgramUpdate->LinkProgram();

        mPtrDefaultProgramRender = ResourceManager::GetShader(PARTICLE_STRAFE_RENDER_PROGRAM);
        mPtrDefaultProgramRender->LinkProgram();

        mPtrOptionalProgramRender = ResourceManager::GetShader(PARTICLE_EXPLODE_RENDER_PROGRAM);
        mPtrOptionalProgramRender->LinkProgram();

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

            for (uint32_t j = 0; j != mIntNumParticleAttributes; ++j)
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
        mState = State::READY;

        return true;
    }

    void Firework::Update(float deltaTime) {
        if (!mBInitialized)return;

        mPtrDefaultProgramUpdate->Use();

        switch (mState)
        {
            case State::READY:
            {
                if(FireworksDemoEngine::GetInstance()->CheckMouseButton())
                {
                    mPtrDefaultProgramUpdate->Use();

                    float posX = GLFWWrapper::GetInstance()->GetWidth() * 0.5f;
                    float posY = 0.f;

                    glm::vec3 color = glm::vec3(Random::getRandomZeroToOne(), Random::getRandomZeroToOne(), Random::getRandomZeroToOne());
                    float minLife = 0.2f;
                    float maxLife = Random::getRandomZeroToOne() + minLife;
                    float size = 2.f + 3.f * Random::getRandomZeroToOne();

                    SetEmitterParameters(glm::vec2(posX, posY),
                                         glm::vec2(-50, -50),
                                         glm::vec2(50, 50),
                                         glm::vec2(0.f, -20.f),
                                         color,
                                         minLife,
                                         maxLife,
                                         size,
                                         0.5f,
                                         1);

                    calculateStartParameters(FireworksDemoEngine::GetInstance()->GetMousePressedPosition(), 1.5f);
                    mPtrDefaultProgramUpdate->SetUniform("mVec2EmitPosition", mVec2EmitPosition);
                    mPtrDefaultProgramUpdate->SetUniform("mIntNumTargetEmission", mIntNumTargetEmission);
                    mState = State::SHOOT;
                }
                break;
            }

            case State::DEAD:
                break;


            case State::SHOOT:
            {
                mPtrDefaultProgramUpdate->Use();

                mVec2EmitPosition.x += mVec2EmitterVelocity.x * deltaTime;

                mVec2EmitterVelocity.y += mVec2Acceleration.y * deltaTime;
                mVec2EmitPosition.y += mVec2EmitterVelocity.y * deltaTime;

                glm::vec3 vRandomSeed;
                vRandomSeed.x = Random::getRandomZeroToOne();
                vRandomSeed.y = Random::getRandomZeroToOne();
                vRandomSeed.z = Random::getRandomZeroToOne();
                mPtrDefaultProgramUpdate->SetUniform("vRandomSeed", vRandomSeed);

                mPtrDefaultProgramUpdate->SetUniform("fTimePassed", deltaTime);
                mPtrDefaultProgramUpdate->SetUniform("mVec2EmitterVelocity", mVec2EmitterVelocity);
                mPtrDefaultProgramUpdate->SetUniform("mVec2EmitPosition", mVec2EmitPosition);
                mPtrDefaultProgramUpdate->SetUniform("mVec2EmitVelocityMin", mVec2EmitVelocityMin);
                mPtrDefaultProgramUpdate->SetUniform("mVec2EmitVelocityRange", mVec2EmitVelocityRange);
                mPtrDefaultProgramUpdate->SetUniform("mVec3EmitColor", mVec3EmitColor);
                mPtrDefaultProgramUpdate->SetUniform("mVec2EmitGravity", mVec2EmitGravity);

                mPtrDefaultProgramUpdate->SetUniform("mFEmitLifeTimeMin", mFEmitLifeTimeMin);
                mPtrDefaultProgramUpdate->SetUniform("mFEmitLifeTimeRange", mFEmitLifeTimeRange);

                mPtrDefaultProgramUpdate->SetUniform("mFEmitSize", mFEmitSize);

                if(checkTargetDistance())
                {
                    mState = State::START_EXPLODE;
                }
                break;
            }


            case State::START_EXPLODE:
            {
                mFDeltaTime = 0.f;
                mPtrOptionalProgramUpdate->Use();

                glm::vec3 color = glm::vec3(Random::getRandomZeroToOne(), Random::getRandomZeroToOne(), Random::getRandomZeroToOne());
                float minLife = 2.f;
                float maxLife = Random::getRandomZeroToOne() + minLife;
                float size = 2.f * Random::getRandomZeroToOne() + 2.f;
                float maxVelocity = 200.f * Random::getRandomZeroToOne();
                int32_t count = Random::getRandom(200, 400);

                SetEmitterParameters(mVec2ExplodePosition,
                                     glm::vec2(-maxVelocity, -maxVelocity),
                                     glm::vec2(maxVelocity, maxVelocity),
                                     glm::vec2(0.f, -20.f),
                                     color,
                                     minLife,
                                     maxLife,
                                     size,
                                     0.f,
                                     count);

                mPtrOptionalProgramUpdate->SetUniform("mIntNumTargetEmission", mIntNumTargetEmission);

                glm::vec3 vRandomSeed;
                vRandomSeed.x = Random::getRandomZeroToOne();
                vRandomSeed.y = Random::getRandomZeroToOne();
                vRandomSeed.z = Random::getRandomZeroToOne();
                mPtrOptionalProgramUpdate->SetUniform("vRandomSeed", vRandomSeed); //adress of vRandomSeed ??

                mPtrOptionalProgramUpdate->SetUniform("fTimePassed", deltaTime);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitterVelocity", mVec2EmitterVelocity);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitPosition", mVec2EmitPosition);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitVelocityMin", mVec2EmitVelocityMin);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitVelocityRange", mVec2EmitVelocityRange);
                mPtrOptionalProgramUpdate->SetUniform("mVec3EmitColor", mVec3EmitColor);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitGravity", mVec2EmitGravity);

                mPtrOptionalProgramUpdate->SetUniform("mFEmitLifeTimeMin", mFEmitLifeTimeMin);
                mPtrOptionalProgramUpdate->SetUniform("mFEmitLifeTimeRange", mFEmitLifeTimeRange);

                mPtrOptionalProgramUpdate->SetUniform("mFEmitSize", mFEmitSize);

                mState = State::EXPLODING;
                break;
            }

            case State::EXPLODING:
            {
                mFDeltaTime += deltaTime;

                mPtrOptionalProgramUpdate->Use();
                mPtrOptionalProgramUpdate->SetUniform("mIntNumTargetEmission", 0);

                glm::vec3 vRandomSeed;
                vRandomSeed.x = Random::getRandomZeroToOne();
                vRandomSeed.y = Random::getRandomZeroToOne();
                vRandomSeed.z = Random::getRandomZeroToOne();
                mPtrOptionalProgramUpdate->SetUniform("vRandomSeed", vRandomSeed);

                mPtrOptionalProgramUpdate->SetUniform("fTimePassed", deltaTime);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitterVelocity", mVec2EmitterVelocity);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitPosition", mVec2EmitPosition);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitVelocityMin", mVec2EmitVelocityMin);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitVelocityRange", mVec2EmitVelocityRange);
                mPtrOptionalProgramUpdate->SetUniform("mVec3EmitColor", mVec3EmitColor);
                mPtrOptionalProgramUpdate->SetUniform("mVec2EmitGravity", mVec2EmitGravity);

                mPtrOptionalProgramUpdate->SetUniform("mFEmitLifeTimeMin", mFEmitLifeTimeMin);
                mPtrOptionalProgramUpdate->SetUniform("mFEmitLifeTimeRange", mFEmitLifeTimeRange);

                mPtrOptionalProgramUpdate->SetUniform("mFEmitSize", mFEmitSize);
                if(mIntCurNumParticles == 1 || mFDeltaTime > (mFEmitLifeTimeMin + mFEmitLifeTimeRange + 4.f))
                {
                    float posX = GLFWWrapper::GetInstance()->GetWidth() * 0.5f;
                    float posY = 0.f;
                    mVec2EmitPosition = {posX, posY};
                    mState = State::DEAD;
                }
                break;
            }
        }

        switch (mState)
        {
            case State::SHOOT:
            case State::START_EXPLODE:
            case State::EXPLODING:
                glEnable(GL_RASTERIZER_DISCARD);
                glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, mUintTFBuffer);

                glBindVertexArray(mUintVAO[mIntCurParticleBuffer]);
                glEnableVertexAttribArray(1); // Re-enable velocity

                ParticleSystem::SwapParticleBuffers();
                glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, mUintParticleBuffer[mIntCurParticleBuffer]);
                glBeginTransformFeedback(GL_POINTS);

                glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, mUintQueryParticleNum);

                glDrawArrays(GL_POINTS, 0, mIntCurNumParticles);

                glEndTransformFeedback();

                glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
                glGetQueryObjectiv(mUintQueryParticleNum, GL_QUERY_RESULT, &mIntCurNumParticles);
                std::cout << mIntCurNumParticles << "\n";
                glBindVertexArray(0);
                glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

                glDisable(GL_RASTERIZER_DISCARD);
                break;

            case State::READY:
            case State::DEAD:
                return;
        }

    }

    void Firework::Render()
    {
        if (!mBInitialized)return;

        switch (mState)
        {
            case State::SHOOT:
                mPtrDefaultProgramRender->Use();
                mPtrDefaultProgramRender->SetUniform("umProjection", mMat4Projection);
                mPtrDefaultProgramRender->SetUniform("gSampler", 0);
                break;

            case State::START_EXPLODE:
            case State::EXPLODING:
                mPtrOptionalProgramRender->Use();
                mPtrOptionalProgramRender->SetUniform("umProjection", mMat4Projection);
                mPtrOptionalProgramRender->SetUniform("gSampler", 0);
                break;

            case State::READY:
            case State::DEAD:
                return;
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE);

        mPtrTextureParticle->Bind();

        glBindVertexArray(mUintVAO[mIntCurParticleBuffer]);
        glDisableVertexAttribArray(1); // Disable velocity, because we don't need it for rendering

        glDrawArrays(GL_POINTS, 0, mIntCurNumParticles);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
    }

    const glm::vec2 &Firework::getMExplodePosition() const
    {
        return mVec2ExplodePosition;
    }
    void Firework::calculateStartParameters(const glm::vec2 &mVec2ExplodePosition, float time)
    {
        Firework::mVec2ExplodePosition = mVec2ExplodePosition;
        auto distanceX = static_cast<float>(fabs(mVec2ExplodePosition.x - mVec2EmitPosition.x));
        auto distanceY = static_cast<float>(fabs(mVec2ExplodePosition.y - mVec2EmitPosition.y));

        mVec2EmitterVelocity.x = distanceX / time;
        mVec2EmitterVelocity.y = 2 * distanceY / time;
        mVec2EmitterVelocity.x = mVec2ExplodePosition.x - mVec2EmitPosition.x > 0.f ? mVec2EmitterVelocity.x : -mVec2EmitterVelocity.x;
        mVec2EmitterVelocity.y = mVec2ExplodePosition.y - mVec2EmitPosition.y > 0.f ? mVec2EmitterVelocity.y : -mVec2EmitterVelocity.y;
        mVec2Acceleration.y = -(mVec2EmitterVelocity.y / time);
    }

    bool Firework::checkTargetDistance() const {
        float dX = static_cast<float>(fabs(mVec2EmitPosition.x - mVec2ExplodePosition.x));
        float dY = static_cast<float>(fabs(mVec2EmitPosition.y - mVec2ExplodePosition.y));
//        float sqrX = dX * dX;
//        float sqrY = dY * dY;
//        float distance = sqrtf(sqrX + sqrY);
        return dX - 20.f < 0.f && dY - 20.f < 0.f;
//        return distance - 8.f < 0.f;
    }

    void Firework::Reset() {
        mState = State::READY;
    }

    bool Firework::isAlive() const {
        return mState != State::DEAD;
    }


}