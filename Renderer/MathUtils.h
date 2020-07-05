#pragma once

#include <glm/glm.hpp>

template<typename T>
const T& clamp(const T& v, const T& lo, const T& hi)
{
	assert(lo <= hi);

	if (v < lo)
	{
		return lo;
	}
	else if (v > hi)
	{
		return hi;
	}
	else
	{
		return v;
	}
}

template<typename T>
T lerp(const T& a, const T& b, const T& x)
{
	// FMA friendly
	return x * b + (a - a * x);
}

/**
 * \brief Map a 3D point with a transformation.
 *        Convert the 3D vector to 4D homogeneous coordinates and back to 3D.
 * \param transformation A 4x4 transformation matrix
 * \param point A 3D point
 * \return The transformed 3D point
 */
glm::vec3 mapPoint(const glm::mat4& transformation, const glm::vec3& point);

/**
 * \brief Map a 3D vector with a transformation.
 *        Convert the 3D vector to 4D homogeneous coordinates and back to 3D.
 * \param transformation A 4x4 transformation matrix
 * \param vector A 3D vector
 * \return The transformed 3D vector
 */
glm::vec3 mapVector(const glm::mat4& transformation, const glm::vec3& vector);
