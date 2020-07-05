#pragma once

#include <glm/glm.hpp>

#include "Material.h"

class Lambertian : public Material
{
public:
	explicit Lambertian(const glm::vec3& color);

	bool scatter(const HitRecord& hit, glm::vec3& color, Ray& scattered) const override;
	
private:
	glm::vec3 m_color;
};

