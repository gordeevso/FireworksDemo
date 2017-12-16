#pragma once

class TimeManager
{
public:
    explicit TimeManager();
    ~TimeManager() = default;
    TimeManager(TimeManager const &) = delete;
    TimeManager & operator=(TimeManager const &) = delete;

    void UpdateMainLoop();
    float GetTimeNow() const noexcept;
    void Reset() noexcept;
    float FrameTime() noexcept;
    float FramesPerSecond() noexcept;
    float GetElapsed() const noexcept;
    void ResetElapsed() noexcept;

private:
    float mElapsed;
    float mLastGetTime;
    float mTimer;
    float mFrameTime;
    float mNowTime;
    float mSleepTime;
    float mFPS;
};

