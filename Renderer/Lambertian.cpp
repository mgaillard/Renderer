#include "Lambertian.h"

#include "Random.h"

Lambertian::Lambertian(const glm::vec3& color) :
	m_color(color)
{
	
}

bool Lambertian::scatter(const HitRecord& hit, glm::vec3& color, Ray& scattered) const
{
	const auto scatterDirection = Random::randomOnUnitHemisphere(hit.normal);

	scattered = Ray(hit.point, scatterDirection);
	
	color = m_color;
	
	return true;
}
