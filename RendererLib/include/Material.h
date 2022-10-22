#pragma once

#include "Random.h"
#include "Ray.h"
#include "Types.h"

class Material
{
public:
	virtual ~Material() = default;

	/**
	 * \brief Simulate the scattering of a ray on the material
	 * \param hit The ray that hit the material
	 * \param randomGenerator A random generator (for reproducibility)
	 * \param attenuation Attenuation of color of the ray
	 * \param scattered The scattered ray
	 * \return True if scattering was successful
	 */
	virtual bool scatter(const HitRecord& hit,
	                     Random& randomGenerator,
	                     Vec3& attenuation,
	                     Ray& scattered) const = 0;
};
