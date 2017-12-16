#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GLFWWrapper
{
public:
    static GLFWWrapper * GetInstance() {
        static GLFWWrapper instance;
        return &instance;
    }

private:
    GLFWWrapper();
    GLFWWrapper(GLFWWrapper const &) = delete;
    GLFWWrapper & operator=(GLFWWrapper const &) = delete;

public:
    ~GLFWWrapper();

    bool Init();

    double GetTimeNow() const;

    bool CheckCloseWindow() const;
    void PollEvents() const;
    void SwapBuffers();

    void GetCursorPos(double * posX, double * posY);
    static bool GetMouseButtonState();

    GLFWwindow* const GetWindow() const noexcept;
    GLuint      const GetWidth() const noexcept;
    GLuint      const GetHeight() const noexcept;

private:
    //callback which will be called during PollEvents() in main loop
    static void MouseButtonStateCallback(GLFWwindow *window,
                                         int32_t button,
                                         int32_t action,
                                         int32_t mods);
private:
    //window params
    GLuint mWidth;
    GLuint mHeight;
    //pointer to some GLFW stuff like OpenGL context
    GLFWwindow* mPtrWindow;

    //state to handle on start button click
    static bool mMouseButtonPressed;
};