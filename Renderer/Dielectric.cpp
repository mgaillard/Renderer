#include "Dielectric.h"

Dielectric::Dielectric(float refractionIndex) :
	m_refractionIndex(refractionIndex)
{
	
}

bool Dielectric::scatter(const HitRecord& hit, glm::vec3& attenuation, Ray& scattered) const
{
    attenuation = glm::vec3(1.0, 1.0, 1.0);

	// When hitting the surface from outside to inside
    glm::vec3 normal = hit.normal;
    float refractionIndexRatio = 1.f / m_refractionIndex;

	// When hitting the surface from inside to outside
	if (!hit.isFrontFace)
    {
        refractionIndexRatio = m_refractionIndex;
        normal = -hit.normal;
    }

    const auto unitDirection = glm::normalize(hit.ray.direction());

    const double cosTheta = std::fmin(glm::dot(-unitDirection, normal), 1.0);
    const double sinTheta = sqrt(1.0 - cosTheta * cosTheta);

    if (refractionIndexRatio * sinTheta > 1.0)
    {
        const glm::vec3 reflected = glm::reflect(unitDirection, hit.normal);
        scattered = Ray(hit.point, reflected);
        return true;
    }
	
    const auto refracted = glm::refract(unitDirection, normal, refractionIndexRatio);
    scattered = Ray(hit.point, refracted);
    return true;
}
