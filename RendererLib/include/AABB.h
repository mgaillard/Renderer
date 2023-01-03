#pragma once

#include "Ray.h"
#include "Types.h"

class AABB
{
public:
    AABB() = default;

    explicit AABB(const Vec3& a, const Vec3& b);

    /**
     * \brief Return the minimum coordinate on the X axis
     * \return The minimum coordinate on the X axis
     */
    double minX() const { return m_start.x; }

    /**
     * \brief Return the minimum coordinate on the Y axis
     * \return The minimum coordinate on the Y axis
     */
    double minY() const { return m_start.y; }

    /**
     * \brief Return the minimum coordinate on the Z axis
     * \return The minimum coordinate on the Z axis
     */
    double minZ() const { return m_start.z; }

    /**
     * \brief Return the point whose coordinates are minimum
     * \return The point whose coordinates are minimum
     */
    const Vec3& min() const { return m_start; }

    /**
     * \brief Return the maximum coordinate on the X axis
     * \return The maximum coordinate on the X axis
     */
    double maxX() const { return m_end.x; }

    /**
     * \brief Return the maximum coordinate on the Y axis
     * \return The maximum coordinate on the Y axis
     */
    double maxY() const { return m_end.y; }

    /**
     * \brief Return the maximum coordinate on the Z axis
     * \return The maximum coordinate on the Z axis
     */
    double maxZ() const { return m_end.z; }

    /**
     * \brief Return the point whose coordinates are maximum
     * \return The point whose coordinates are maximum
     */
    const Vec3& max() const { return m_end; }

    /**
     * \brief Return the size on the X axis
     * \return The size on the X axis
     */
    double sizeX() const { return m_end.x - m_start.x; }

    /**
     * \brief Return the size on the Y axis
     * \return The size on the Y axis
     */
    double sizeY() const { return m_end.y - m_start.y; }

    /**
     * \brief Return the size on the Z axis
     * \return The size on the Z axis
     */
    double sizeZ() const { return m_end.z - m_start.z; }

    /**
     * \brief Return the center of the AABB
     * \return The center of the AABB
     */
    [[nodiscard]] Vec3 center() const { return (m_start + m_end) / 2.0; }

private:

    /**
     * \brief Minimum coordinates of the AABB
     */
    Vec3 m_start;

    /**
     * \brief Maximum coordinates of the AABB
     */
    Vec3 m_end;
};

/**
 * \brief Ray AABB box intersection
 * \param box The AABB box
 * \param ray The Ray
 * \param t Time on the ray of the first hit
 * \return True if the ray intersects the AABB
 */
bool rayAABBIntersection(const AABB& box, const Ray& ray, double& t);

/**
 * \brief Ray AABB box intersection
 * \param box The AABB box
 * \param ray The Ray
 * \return True if the ray intersects the AABB
 */
bool rayAABBIntersection(const AABB& box, const Ray& ray);
