#include "Dielectric.h"

#include "Random.h"

Dielectric::Dielectric(double refractionIndex) :
	m_refractionIndex(refractionIndex)
{
	
}

double schlick(double cosine, double refractionIndex)
{
    auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

bool Dielectric::scatter(const HitRecord& hit, Random& randomGenerator, Vec3& attenuation, Ray& scattered) const
{
    attenuation = Vec3(1.0, 1.0, 1.0);

	// When hitting the surface from outside to inside
    Vec3 normal = hit.normal;
    double refractionIndexRatio = 1.0 / m_refractionIndex;

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
        const auto reflected = glm::reflect(unitDirection, hit.normal);
        scattered = Ray(hit.point, reflected);
        return true;
    }

    const double reflectProb = schlick(cosTheta, refractionIndexRatio);
    if (randomGenerator.randomNumberUnit() < reflectProb)
    {
        const auto reflected = glm::reflect(unitDirection, hit.normal);
        scattered = Ray(hit.point, reflected);
        return true;
    }
	
    const auto refracted = glm::refract(unitDirection, normal, refractionIndexRatio);
    scattered = Ray(hit.point, refracted);
    return true;
}
