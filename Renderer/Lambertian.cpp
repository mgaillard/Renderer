#include "Lambertian.h"

#include "Random.h"

Lambertian::Lambertian(const Vec3& albedo) :
	m_albedo(albedo)
{
	
}

bool Lambertian::scatter(const HitRecord& hit, Vec3& attenuation, Ray& scattered) const
{
	const auto scatterDirection = Random::randomOnUnitHemisphere(hit.normal);

	scattered = Ray(hit.point, scatterDirection);
	
	attenuation = m_albedo;
	
	return true;
}
