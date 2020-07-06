#pragma once

#include <glm/glm.hpp>

#include "Material.h"

class Metal : public Material
{
public:
	explicit Metal(const glm::vec3& albedo, float fuzz);

	bool scatter(const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override;

private:
	glm::vec3 m_albedo;

	float m_fuzz;
};

