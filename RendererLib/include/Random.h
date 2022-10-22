#pragma once

#include <random>

#include "Types.h"

class Random
{
public:
    using GeneratorType = std::mt19937_64;
    using SeedType = GeneratorType::result_type;

    explicit Random(SeedType seed = GeneratorType::default_seed);

    // The random object is non-copyable but movable to ensure the random generator is consistent
    Random(Random const&) = delete;
    Random& operator=(Random const&) = delete;
    Random(Random&&) = default;
    Random& operator=(Random&&) = default;

    /**
     * \brief Return a random float number in [-0.5; 0.5]
     * \return A random float number in [-0.5; 0.5]
     */
    double randomNumber();

    /**
     * \brief Return a random float number in [0.0; 1.0]
     * \return A random float number in [0.0; 1.0]
     */
    double randomNumberUnit();

    /**
     * \brief Generate a random float number in an interval
     * \param a Minimum value
     * \param b Maximum value
     * \return The random float number in the interval [a; b]
     */
    double randomNumberInterval(double a, double b);

    /**
     * \brief Generate a random point on a unit sphere
     * \return A random point on a unit sphere
     */
    Vec3 randomOnUnitSphere();

    /**
     * \brief Generate a random point on a unit hemisphere
     * \param normal The direction of the hemisphere
     * \return A random point on a unit hemisphere
     */
    Vec3 randomOnUnitHemisphere(const Vec3& normal);

    /**
     * \brief Create multiple random generators with a starting seed
     *        The first random generator has seed = startingSeed
     *		  The second random generator has seed = startingSeed + 1
     *		  The third random generator has seed = startingSeed + 2
     * \param n The number of random generators to create
     * \param startingSeed The starting seed, that will be the seed of the first generator
     * \return An array of random generators
     */
    static [[nodiscard]] std::vector<Random> createMultiple(int n, SeedType startingSeed);
    
private:
    GeneratorType m_randomGenerator;
};
