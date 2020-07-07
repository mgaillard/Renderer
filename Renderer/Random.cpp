#include "Random.h"

#include <random>

#include <glm/gtc/random.hpp>

std::mt19937 Random::m_randomGenerator = std::mt19937();

float Random::randomNumber()
{
	std::uniform_real_distribution<float> distribution(-0.5f, 0.5f);
	return distribution(m_randomGenerator);
}

float Random::randomNumberUnit()
{
	std::uniform_real_distribution<float> distribution(0.f, 1.f);
	return distribution(m_randomGenerator);
}

glm::vec3 Random::randomOnUnitHemisphere(const glm::vec3& normal)
{
	const auto pointOnUnitSphere = glm::sphericalRand(1.f);

	// If in the same hemisphere as the normal
	if (glm::dot(pointOnUnitSphere, normal) > 0.f)
	{
		return pointOnUnitSphere;
	}
	else
	{
		return -pointOnUnitSphere;
	}
}
