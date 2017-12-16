#include <iostream>
#include "FireworksDemoEngine.hpp"
#include "GLFWWrapper.hpp"
#include "ResourceManager.hpp"

int main()
{
    try
    {
        GLFWWrapper::GetInstance()->Init();
        ResourceManager::LoadResources();
        FireworksDemoEngine::GetInstance()->Run();
        return 0;
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    catch (...)
    {
        std::cerr << "Unknown error" << std::endl;
        return -1;
    }
}
