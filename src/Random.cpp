#include <chrono>
#include "Random.hpp"


std::default_random_engine Random::msGenerator(static_cast<uint32_t>(std::chrono::system_clock::now().time_since_epoch().count()));
std::uniform_real_distribution<float> Random::msRealDistribution(-100.f, 100.f);
std::normal_distribution<float> Random::msNormalDistribution(0.f, 1.f);

float Random::getRandomZeroToOne()
{
    return msNormalDistribution(msGenerator);
}

uint32_t Random::getRandom(uint32_t from, uint32_t to)
{
    return (msGenerator() % (to - from + 1)) + from;
}
