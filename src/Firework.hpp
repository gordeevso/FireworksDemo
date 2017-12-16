#pragma once

#include "ParticleSystem.hpp"

namespace Particles {

    class Firework : public ParticleSystem
    {
    public:
        enum class State
        {
            READY,
            SHOOT,
            START_EXPLODE,
            EXPLODING,
            DEAD
        };

    public:
        explicit Firework(int32_t maxParticles);
        bool Init() override;
        void Update(float deltaTime) override;
        void Render() override;
        void Reset();
        bool isAlive() const;

        const glm::vec2 &getMExplodePosition() const;
        void calculateStartParameters(const glm::vec2 &mVec2ExplodePosition, float time);
        bool checkTargetDistance() const;

    private:
        State  mState;
        glm::vec2 mVec2ExplodePosition;
        glm::vec2 mVec2Acceleration;
        glm::vec2 mVec2EmitterVelocity;
        std::shared_ptr<Shader> mPtrOptionalProgramUpdate;
        std::shared_ptr<Shader> mPtrOptionalProgramRender;
    };

}


