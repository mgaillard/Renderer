#pragma once

#include "Ray.h"
#include "Types.h"

class Material
{
public:
	virtual ~Material() = default;
	
	virtual bool scatter(const HitRecord& hit, Vec3& attenuation, Ray& scattered) const = 0;
};
