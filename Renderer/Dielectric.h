#pragma once

#include <glm/glm.hpp>

#include "Material.h"

class Dielectric : public Material
{
public:
	explicit Dielectric(float refractionIndex);

	bool scatter(const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const override;

private:
	float m_refractionIndex;
};
