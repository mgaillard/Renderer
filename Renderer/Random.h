#pragma once

#include <random>

#include <glm/glm.hpp>

class Random
{
public:

	Random() = delete;
	Random(Random const&) = delete;
	Random(Random&&) = delete;
	Random& operator=(Random const&) = delete;
	Random& operator=(Random&&) = delete;

	/**
	 * \brief Return a random float number in [-0.5; 0.5]
	 * \return A random float number in [-0.5; 0.5]
	 */
	static float randomNumber();

	/**
	 * \brief Return a random float number in [0.0; 1.0]
	 * \return A random float number in [0.0; 1.0]
	 */
	static float randomNumberUnit();

	/**
	 * \brief Generate a random point on a unit hemisphere
	 * \param normal The direction of the hemisphere
	 * \return A random point on a unit hemisphere
	 */
	static glm::vec3 randomOnUnitHemisphere(const glm::vec3& normal);
	
private:
	static std::mt19937 m_randomGenerator;
};

