#include <iostream>

#include "GLFWWrapper.hpp"

bool GLFWWrapper::mMouseButtonPressed = false;

GLFWWrapper::GLFWWrapper() : mWidth{800},
                            mHeight{600}
{}

GLFWWrapper::~GLFWWrapper() {
    glfwTerminate();
}

bool GLFWWrapper::Init()  {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    mPtrWindow = glfwCreateWindow(mWidth, mHeight, "FireworksDemo", nullptr, nullptr);
    if (!mPtrWindow)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(mPtrWindow);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    glViewport(0, 0, mWidth, mHeight);

    glfwSetMouseButtonCallback(mPtrWindow, MouseButtonStateCallback);
    return true;
}

bool GLFWWrapper::CheckCloseWindow() const {
    return static_cast<bool>(glfwWindowShouldClose(mPtrWindow));
}

void GLFWWrapper::PollEvents() const {
    glfwPollEvents();
}

void GLFWWrapper::SwapBuffers() {
    glfwSwapBuffers(mPtrWindow);
}

GLFWwindow * const GLFWWrapper::GetWindow() const noexcept {
    return mPtrWindow;
}

GLuint const GLFWWrapper::GetWidth() const noexcept {
    return mWidth;
}

GLuint const GLFWWrapper::GetHeight() const noexcept {
    return mHeight;
}

void GLFWWrapper::GetCursorPos(double * posX, double * posY) {
    glfwGetCursorPos(mPtrWindow, posX, posY);
    *posY = mHeight - *posY;
}

void GLFWWrapper::MouseButtonStateCallback(GLFWwindow *window,
                                           int32_t button,
                                           int32_t action,
                                           int32_t mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        mMouseButtonPressed = true;
    }
    else
    {
        mMouseButtonPressed = false;
    }
}

bool GLFWWrapper::GetMouseButtonState()
{
    if(mMouseButtonPressed)
    {
        mMouseButtonPressed = false;
        return true;
    }
    return false;
}

double GLFWWrapper::GetTimeNow() const
{
    return glfwGetTime();
}





