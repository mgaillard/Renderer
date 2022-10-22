#pragma once

#include "Material.h"
#include "Types.h"

class Dielectric : public Material
{
public:
	explicit Dielectric(double refractionIndex);

	bool scatter(const HitRecord& hit, Random& randomGenerator, Vec3& attenuation, Ray& scattered) const override;

private:
	double m_refractionIndex;
};
