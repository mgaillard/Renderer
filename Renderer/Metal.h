#pragma once

#include "Material.h"
#include "Types.h"

class Metal : public Material
{
public:
	explicit Metal(const Vec3& albedo, double fuzz);

	bool scatter(const HitRecord& hit, Vec3& attenuation, Ray& scattered) const override;

private:
	Vec3 m_albedo;

	double m_fuzz;
};

