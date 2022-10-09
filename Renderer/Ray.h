#pragma once

#include <memory>

#include <glm/glm.hpp>

// Forward declaration
class Material;

class Ray
{
public:

	Ray() :
		m_origin(0.f, 0.f, 0.f),
		m_direction(0.f, 0.f, 0.f)
	{
		
	}

	Ray(const glm::vec3& origin, const glm::vec3& direction) :
		m_origin(origin),
		m_direction(direction)
	{
		
	}

	const glm::vec3& origin() const
	{
		return m_origin;
	}

	const glm::vec3& direction() const
	{
		return m_direction;
	}

	[[nodiscard]] glm::vec3 at(float t) const
	{
		return m_origin + m_direction * t;
	}

private:
	glm::vec3 m_origin;
	glm::vec3 m_direction;
};

class HitRecord
{
public:
	// Incoming ray on the hit
	Ray ray;
	// Distance to the origin of the ray until the hit
	float t;
	// Point in world coordinates of the hit 
	glm::vec3 point;
	// Normal of the surface hit by the ray in world coordinates
	glm::vec3 normal;
	// True if the hit is a front face
	bool isFrontFace;
	// Material hit by the ray
	std::shared_ptr<Material> material;
};