#include "AABB.h"

#include <algorithm>

AABB::AABB(const Vec3& a, const Vec3& b) :
    m_start(std::min(a.x, b.x),
            std::min(a.y, b.y),
            std::min(a.z, b.z)),
    m_end(std::max(a.x, b.x),
          std::max(a.y, b.y),
          std::max(a.z, b.z))
{

}

bool rayAABBIntersection(const AABB& box, const Ray& ray)
{
    // Source: https://www.scratchapixel.com
    //         A Minimal Ray-Tracer: Rendering Simple Shapes (Sphere, Cube, Disk, Plane, etc.)

    const auto invDirection = ray.inverseDirection();
    double txmin, txmax, tymin, tymax, tzmin, tzmax;

    if (invDirection.x >= 0.0)
    {
        txmin = (box.minX() - ray.origin().x) * invDirection.x;
        txmax = (box.maxX() - ray.origin().x) * invDirection.x;
    }
    else {
        txmin = (box.maxX() - ray.origin().x) * invDirection.x;
        txmax = (box.minX() - ray.origin().x) * invDirection.x;
    }

    if (invDirection.y >= 0.0)
    {
        tymin = (box.minY() - ray.origin().y) * invDirection.y;
        tymax = (box.maxY() - ray.origin().y) * invDirection.y;
    }
    else {
        tymin = (box.maxY() - ray.origin().y) * invDirection.y;
        tymax = (box.minY() - ray.origin().y) * invDirection.y;
    }

    if ((txmin > tymax) || (tymin > txmax))
        return false;

    if (tymin > txmin)
        txmin = tymin;

    if (tymax < txmax)
        txmax = tymax;
    
    if (invDirection.z >= 0.0)
    {
        tzmin = (box.minZ() - ray.origin().z) * invDirection.z;
        tzmax = (box.maxZ() - ray.origin().z) * invDirection.z;
    }
    else {
        tzmin = (box.maxZ() - ray.origin().z) * invDirection.z;
        tzmax = (box.minZ() - ray.origin().z) * invDirection.z;
    }

    if ((txmin > tzmax) || (tzmin > txmax))
        return false;

    if (tzmin > txmin)
        txmin = tzmin;

    if (tzmax < txmax)
        txmax = tzmax;

    return true;
}
