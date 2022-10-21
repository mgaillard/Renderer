#pragma once

#include "Material.h"
#include "Types.h"

class Lambertian : public Material
{
public:
	explicit Lambertian(const Vec3& albedo);

	bool scatter(const HitRecord& hit, Vec3& attenuation, Ray& scattered) const override;
	
private:
	Vec3 m_albedo;
};
