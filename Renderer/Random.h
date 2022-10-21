#pragma once

#include <random>

#include "Types.h"

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
	static double randomNumber();

	/**
	 * \brief Return a random float number in [0.0; 1.0]
	 * \return A random float number in [0.0; 1.0]
	 */
	static double randomNumberUnit();

	/**
	 * \brief Generate a random float number in an interval
	 * \param a Minimum value
	 * \param b Maximum value
	 * \return The random float number in the interval [a; b]
	 */
	static double randomNumberInterval(double a, double b);

	/**
	 * \brief Generate a random point on a unit hemisphere
	 * \param normal The direction of the hemisphere
	 * \return A random point on a unit hemisphere
	 */
	static Vec3 randomOnUnitHemisphere(const Vec3& normal);
	
private:
	static std::mt19937 m_randomGenerator;
};

