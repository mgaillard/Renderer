#pragma once

#include "Ray.h"

class Material
{
public:
	virtual ~Material() = default;
	
	virtual bool scatter(const HitRecord& hit, glm::vec3& color, Ray& scattered) const = 0;
};

