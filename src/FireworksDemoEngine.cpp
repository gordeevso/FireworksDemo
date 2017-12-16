#include <iostream>
#include "FireworksDemoEngine.hpp"
#include "GLFWWrapper.hpp"
#include "Random.hpp"
#include "ResourceManager.hpp"

float const FPS_UPDATE_LIMIT_TIME = 0.05f;



FireworksDemoEngine::FireworksDemoEngine() :
        mMouseX(0.f),
        mMouseY(0.f),
        mMouseButtonPressed(false),
        mPtrTimeManager(new TimeManager),
        mPtrScene(new Scene),
        mPtrTextRenderer(new TextRenderer),
        mPtrPostEffects(new PostEffects)
{}

void FireworksDemoEngine::Run()
{

    mPtrTimeManager->UpdateMainLoop();
    float lastFPS {};

    while(!(GLFWWrapper::GetInstance()->CheckCloseWindow()))
    {
        //update frame time based on constand target fps
        mPtrTimeManager->UpdateMainLoop();

        //polling events like mouse button click
        GLFWWrapper::GetInstance()->PollEvents();

        mMouseButtonPressed = GLFWWrapper::GetInstance()->GetMouseButtonState();
        if(mMouseButtonPressed)
        {
            GLFWWrapper::GetInstance()->GetCursorPos(&mMouseX, &mMouseY);
        }
        //calculating all moves
        mPtrScene->Update(mPtrTimeManager->FrameTime());

        mPtrPostEffects->PreRender();

        mPtrScene->Draw();
        mPtrPostEffects->PostRender();

        //updating fps and smoothly
        if(mPtrTimeManager->GetElapsed() > FPS_UPDATE_LIMIT_TIME)
        {
            DrawFPS(std::to_string(static_cast<int32_t>(mPtrTimeManager->FramesPerSecond())));
            lastFPS = mPtrTimeManager->FramesPerSecond();
            mPtrTimeManager->ResetElapsed();
        }
        else
        {
            DrawFPS(std::to_string(static_cast<int32_t>(lastFPS)));
        }

        GLFWWrapper::GetInstance()->SwapBuffers();
    }
}

void FireworksDemoEngine::DrawFPS(std::string fps)
{
    mPtrTextRenderer->RenderText(fps,
                                 GLFWWrapper::GetInstance()->GetWidth() - 50.f,
                                 GLFWWrapper::GetInstance()->GetHeight() - 40.f,
                                 1.0f,
                                 glm::vec3(0.2f, 0.2f, 0.8f));
}

bool FireworksDemoEngine::CheckMouseButton() const
{
    return mMouseButtonPressed;
}

glm::vec2 FireworksDemoEngine::GetMousePressedPosition() const
{
    return glm::vec2(mMouseX, mMouseY);
}

