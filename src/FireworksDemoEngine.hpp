#pragma once

#include <memory>

#include "TimeManager.hpp"
#include "Scene.hpp"
#include "TextRenderer.hpp"
#include "RenderToTexture.hpp"
#include "PostEffects.hpp"

class FireworksDemoEngine {
public:
    static FireworksDemoEngine * GetInstance() noexcept {
        static FireworksDemoEngine instance;
        return &instance;
    }

private:
    FireworksDemoEngine();
    FireworksDemoEngine(FireworksDemoEngine const &) = delete;
    FireworksDemoEngine & operator=(FireworksDemoEngine const &) = delete;


public:
    ~FireworksDemoEngine() = default;

    void Run();
    void DrawFPS(std::string fps);
    bool CheckMouseButton() const;
    glm::vec2 GetMousePressedPosition() const;

private:
    double mMouseX;
    double mMouseY;
    bool mMouseButtonPressed;
    std::shared_ptr<TimeManager> mPtrTimeManager;
    std::shared_ptr<Scene> mPtrScene;
    std::shared_ptr<TextRenderer> mPtrTextRenderer;
    std::shared_ptr<RenderToTexture> mPtrRenderToTexture;
    std::shared_ptr<PostEffects> mPtrPostEffects;
};
