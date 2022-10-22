#include "Random.h"

#include <random>

#include "Constants.h"

Random::Random(SeedType seed) :
    m_randomGenerator(seed)
{

}

double Random::randomNumber()
{
    return randomNumberInterval(-0.5, 0.5);
}

double Random::randomNumberUnit()
{
    return randomNumberInterval(0.0, 1.0);
}

double Random::randomNumberInterval(double a, double b)
{
    std::uniform_real_distribution<double> distribution(a, b);
    return distribution(m_randomGenerator);
}

Vec3 Random::randomOnUnitSphere()
{
    const double theta = randomNumberInterval(0.0, 2.0 * constants::pi);
    const double phi = std::acos(randomNumberInterval(-1.0, 1.0));

    const double x = std::sin(phi) * std::cos(theta);
    const double y = std::sin(phi) * std::sin(theta);
    const double z = std::cos(phi);

    return { x, y, z };
}

Vec3 Random::randomOnUnitHemisphere(const Vec3& normal)
{
    const auto pointOnUnitSphere = randomOnUnitSphere();

    // If in the same hemisphere as the normal
    if (glm::dot(pointOnUnitSphere, normal) >= 0.0)
    {
        return pointOnUnitSphere;
    }
    else
    {
        return -pointOnUnitSphere;
    }
}

std::vector<Random> Random::createMultiple(int n, SeedType startingSeed)
{
    std::vector<Random> instances;
    instances.reserve(n);

    auto currentSeed = startingSeed;
    for (int i = 0; i < n; i++)
    {
        instances.emplace_back(currentSeed);
        currentSeed++;
    }

    return instances;
}
