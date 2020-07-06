#include "Metal.h"

#include "Random.h"
#include "MathUtils.h"

Metal::Metal(const glm::vec3& albedo, float fuzz) :
	m_albedo(albedo),
	m_fuzz(clamp(fuzz, 0.f, 1.f))
{

}

bool Metal::scatter(const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const
{
	const auto reflectedDirection = glm::reflect(glm::normalize(hit.ray.direction()), hit.normal);
	const auto fuzzReflectedDirection = reflectedDirection + m_fuzz * Random::randomOnUnitHemisphere(hit.normal);

	scattered = Ray(hit.point, fuzzReflectedDirection);

	attenuation = m_albedo;

	return glm::dot(scattered.direction(), hit.normal) > 0.f;
}
