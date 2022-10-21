#include "Random.h"

#include <random>

#include <glm/gtc/random.hpp>

std::mt19937 Random::m_randomGenerator = std::mt19937();

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

Vec3 Random::randomOnUnitHemisphere(const Vec3& normal)
{
	const auto pointOnUnitSphere = glm::sphericalRand(1.0);

	// If in the same hemisphere as the normal
	if (glm::dot(pointOnUnitSphere, normal) > 0.0)
	{
		return pointOnUnitSphere;
	}
	else
	{
		return -pointOnUnitSphere;
	}
}
