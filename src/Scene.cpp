#include <chrono>
#include <algorithm>
#include <vector>
#include <cmath>
#include <iostream>

#include "Scene.hpp"
#include "GLFWWrapper.hpp"
#include "ResourceManager.hpp"
#include "FireworksDemoEngine.hpp"
#include "Random.hpp"


Scene::Scene(size_t maxFireworks)
{
    mCloudSize = 256.f;
    for(size_t i = 0; i != maxFireworks; ++i)
    {
        mReadyFireworkList.emplace_back(std::make_shared<Particles::Firework>(400));
    }

    float randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD1_TEXTURE),{randomX,0},{30,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD2_TEXTURE),{randomX,60},{25,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD3_TEXTURE),{randomX,100},{60,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD4_TEXTURE),{randomX,200},{34,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD5_TEXTURE),{randomX,150},{56,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD6_TEXTURE),{randomX,400},{25,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD7_TEXTURE),{randomX,500},{55,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD8_TEXTURE),{randomX,550},{23,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD9_TEXTURE),{randomX,350},{15,0}));
    randomX = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetWidth());
    mClouds.emplace_back(Object(ResourceManager::GetTexture(CLOUD10_TEXTURE),{randomX,250},{10,0}));
}

void Scene::Draw()
{
    for(const auto & cloud: mClouds)
    {
        mSpriteRenderer.DrawSprite(cloud.texture,cloud.position,{mCloudSize,mCloudSize});
    }

    for(auto const & firework: mActiveFireworkList)
    {
        firework->Render();
    }

}

void Scene::Update(float deltaTime)
{
    for(auto & cloud: mClouds)
    {
        cloud.position += cloud.velocity * deltaTime;
        if(cloud.position.x - GLFWWrapper::GetInstance()->GetWidth() > 0.f)
        {
            cloud.position.x = -mCloudSize;
            cloud.position.y = Random::getRandomZeroToOne() * static_cast<float>(GLFWWrapper::GetInstance()->GetHeight());
        }
    }

    if(FireworksDemoEngine::GetInstance()->CheckMouseButton() && !mReadyFireworkList.empty())
    {
        mActiveFireworkList.push_back(*mReadyFireworkList.begin());
        mReadyFireworkList.erase(mReadyFireworkList.begin());
    }


    for(auto it = mActiveFireworkList.begin(); it != mActiveFireworkList.end(); ++it)
    {
        if(!(*it)->isAlive())
        {
            (*it)->Reset();
            mReadyFireworkList.push_back(*it);
            mActiveFireworkList.erase(it++);
        }
    }

    for(auto & firework: mActiveFireworkList)
    {
        firework->Update(deltaTime);
    }
}
