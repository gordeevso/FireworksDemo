#pragma once

#include <memory>

#include "Shader.hpp"
#include "Texture.hpp"

namespace Particles {

    struct Particle {
        glm::vec2 vPosition = {0.f, 0.f};
        glm::vec2 vVelocity = {0.f, 0.f};
        glm::vec3 vColor = {1.f, 1.f, 1.f};
        float fLifeTime = 2.f;
        float fSize = 1.f;
        int32_t iType = 1;
    };


    class ParticleSystem {
    public:
        explicit ParticleSystem(int32_t maxParticles = 4000);
        virtual ~ParticleSystem();

        virtual bool Init();
        virtual void Render();
        virtual void Update(float fTimePassed);

        virtual void SetEmitterParameters(glm::vec2 a_vGenPosition,
                                          glm::vec2 a_vGenVelocityMin,
                                          glm::vec2 a_vGenVelocityMax,
                                          glm::vec2 a_vGenGravityVector,
                                          glm::vec3 a_vGenColor,
                                          float a_fGenLifeMin,
                                          float a_fGenLifeMax,
                                          float a_fGenSize,
                                          float fEvery,
                                          int32_t a_iNumToGenerate);

        void ClearAllParticles();

        void SwapParticleBuffers() { mIntCurParticleBuffer = 1 - mIntCurParticleBuffer; }

        int32_t GetNumParticles() const;

        const glm::vec2 &getMVec2EmitPosition() const;
        void setMVec2EmitPosition(const glm::vec2 &mVec2EmitPosition);

        const glm::vec2 &getMVec2EmitVelocityMin() const;

        const glm::vec2 &getMVec2EmitVelocityRange() const;
        void setMVec2EmitVelocityRange(const glm::vec2 &mVec2EmitVelocityMin, const glm::vec2 &mVec2EmitVelocityMax);

        const glm::vec2 &getMVec2EmitGravity() const;
        void setMVec2EmitGravity(const glm::vec2 &mVec2EmitGravity);

        const glm::vec3 &getMVec3EmitColor() const;
        void setMVec3EmitColor(const glm::vec3 &mVec3EmitColor);

        float getMFEmitLifeTimeMin() const;

        float getMFEmitLifeTimeRange() const;
        void setMFEmitLifeTimeRange(float mFEmitLifeTimeMin, float mFEmitLifeTimeMax);

        float getMFEmitSize() const;
        void setMFEmitSize(float mFEmitSize);

        int32_t getMIntNumTargetEmission() const;

        void setMIntNumTargetEmission(int32_t mIntNumTargetEmission);

    protected:
        int32_t GetCurrentBuffer() { return mIntCurParticleBuffer; }


    protected:

        std::shared_ptr<Shader> mPtrDefaultProgramRender;
        std::shared_ptr<Shader> mPtrDefaultProgramUpdate;
        std::shared_ptr<Texture> mPtrTextureParticle;

        bool mBInitialized;

        GLuint mUintTFBuffer;
        GLuint mUintParticleBuffer[2];
        GLuint mUintVAO[2];
        GLuint mUintQueryParticleNum;

        int32_t mIntCurParticleBuffer;
        int32_t mIntCurNumParticles;

        glm::mat4 mMat4Projection;

        float mFDeltaTime;
        float mFNextEmissionTime;

        glm::vec2 mVec2EmitPosition;
        glm::vec2 mVec2EmitVelocityMin;
        glm::vec2 mVec2EmitVelocityRange;
        glm::vec2 mVec2EmitGravity;
        glm::vec3 mVec3EmitColor;

        float mFEmitLifeTimeMin;
        float mFEmitLifeTimeRange;
        float mFEmitSize;

        int32_t mIntNumTargetEmission;
        int32_t mIntMaxParticles;
        int32_t mIntNumParticleAttributes;
    };

}