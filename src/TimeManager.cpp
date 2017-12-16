#include <windows.h>

#include "TimeManager.hpp"
#include "GLFWWrapper.hpp"

float const TARGET_FRAME_RATE = 60.f;
float const TARGET_FRAME_TIME = 1.f / TARGET_FRAME_RATE;

TimeManager::TimeManager() :  mElapsed {},
                              mLastGetTime {},
                              mTimer{},
                              mFrameTime{},
                              mNowTime{},
                              mSleepTime{},
                              mFPS{} {
    mLastGetTime = GetTimeNow();
}

void TimeManager::UpdateMainLoop() {
    mTimer += TARGET_FRAME_TIME;

    mNowTime = GetTimeNow();
    mFrameTime = mNowTime - mLastGetTime;
    mLastGetTime = mNowTime;

    mFPS = 1.f / mFrameTime;
    mSleepTime = mTimer - mNowTime;

    mElapsed += mFrameTime;

    if (mSleepTime > 0.f) {
        Sleep(static_cast<uint32_t>(mSleepTime * 1000.f));
    }
}

float TimeManager::GetTimeNow() const noexcept {
    return static_cast<float>(GLFWWrapper::GetInstance()->GetTimeNow());
}

void TimeManager::Reset() noexcept {
    mFrameTime = 0.f;
    mLastGetTime = GetTimeNow();
    mTimer = GetTimeNow();
}

float TimeManager::FrameTime() noexcept {
    return mFrameTime;
}

float TimeManager::FramesPerSecond() noexcept {
    return mFPS;
}

float TimeManager::GetElapsed() const noexcept {
    return mElapsed;
}

void TimeManager::ResetElapsed() noexcept {
    mElapsed = 0.f;
}

