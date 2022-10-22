#include "Metal.h"

#include <algorithm>

#include "Random.h"

Metal::Metal(const Vec3& albedo, double fuzz) :
	m_albedo(albedo),
	m_fuzz(std::clamp(fuzz, 0.0, 1.0))
{

}

bool Metal::scatter(const HitRecord& hit, Random& randomGenerator, Vec3& attenuation, Ray& scattered) const
{
	const auto reflectedDirection = glm::reflect(glm::normalize(hit.ray.direction()), hit.normal);
	const auto fuzzReflectedDirection = reflectedDirection + m_fuzz * randomGenerator.randomOnUnitHemisphere(hit.normal);

	scattered = Ray(hit.point, fuzzReflectedDirection);

	attenuation = m_albedo;

	return glm::dot(scattered.direction(), hit.normal) > 0.0;
}
