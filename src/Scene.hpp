#pragma once

#include <vector>
#include <random>
#include <list>

#include "SpriteRenderer.hpp"
#include "Firework.hpp"

struct Object
{
    std::shared_ptr<Texture> texture;
    glm::vec2 position;
    glm::vec2 velocity;

    Object(std::shared_ptr<Texture> const & tex, glm::vec2 const & pos, glm::vec2 const & vel)
            : texture(tex),
              position(pos),
              velocity(vel)
    {}
};

using FireworkList = std::list<std::shared_ptr<Particles::Firework>>;
using Clouds = std::vector<Object>;

class Scene {
public:
    explicit Scene(size_t maxFireworks = 20);
    ~Scene() = default;

    void Draw();
    void Update(float deltaTime);

private:
//    std::unique_ptr<Particles::Firework> mPtrFirework;
    FireworkList mActiveFireworkList;
    FireworkList mReadyFireworkList;
    Clouds mClouds;
    float mCloudSize;
    //Slot and button drawing
    SpriteRenderer mSpriteRenderer;

    //used for generating target animating time (4 - 5 sec)
    //and generating path for each slot
    std::minstd_rand0 mRandGenerator;
};