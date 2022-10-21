#pragma once

#include <memory>

#include "Types.h"

// Forward declaration
class Material;

class Ray
{
public:

	Ray() :
		m_origin(0.0, 0.0, 0.0),
		m_direction(0.0, 0.0, 0.0)
	{
		
	}

	Ray(const Vec3& origin, const Vec3& direction) :
		m_origin(origin),
		m_direction(direction)
	{
		
	}

	const Vec3& origin() const
	{
		return m_origin;
	}

	const Vec3& direction() const
	{
		return m_direction;
	}

	[[nodiscard]] Vec3 inverseDirection() const
	{
		return 1.0 / m_direction;
	}

	[[nodiscard]] Vec3 at(double t) const
	{
		return m_origin + m_direction * t;
	}

private:
	Vec3 m_origin;
	Vec3 m_direction;
};

class HitRecord
{
public:
	// Incoming ray on the hit
	Ray ray;
	// Distance to the origin of the ray until the hit
	double t;
	// Point in world coordinates of the hit 
	Vec3 point;
	// Normal of the surface hit by the ray in world coordinates
	Vec3 normal;
	// True if the hit is a front face
	bool isFrontFace;
	// Material hit by the ray
	std::shared_ptr<Material> material;
};
