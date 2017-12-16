#include <random>

class Random
{
public:
    static float getRandomZeroToOne();
    static uint32_t getRandom(uint32_t from, uint32_t to);
private:
    static std::default_random_engine msGenerator;
    static std::uniform_real_distribution<float> msRealDistribution;
    static std::normal_distribution<float> msNormalDistribution;
};


