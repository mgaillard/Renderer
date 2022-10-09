#pragma once

#include <glm/glm.hpp>

#include "Ray.h"

class AABB
{
public:
    AABB() = default;

    explicit AABB(const glm::vec3& a, const glm::vec3& b);

    /**
     * \brief Return the minimum coordinate on the X axis
     * \return The minimum coordinate on the X axis
     */
    float minX() const { return m_start.x; }

    /**
     * \brief Return the minimum coordinate on the Y axis
     * \return The minimum coordinate on the Y axis
     */
    float minY() const { return m_start.y; }

    /**
     * \brief Return the minimum coordinate on the Z axis
     * \return The minimum coordinate on the Z axis
     */
    float minZ() const { return m_start.z; }

    /**
     * \brief Return the point whose coordinates are minimum
     * \return The point whose coordinates are minimum
     */
    const glm::vec3& min() const { return m_start; }

    /**
     * \brief Return the maximum coordinate on the X axis
     * \return The maximum coordinate on the X axis
     */
    float maxX() const { return m_end.x; }

    /**
     * \brief Return the maximum coordinate on the Y axis
     * \return The maximum coordinate on the Y axis
     */
    float maxY() const { return m_end.y; }

    /**
     * \brief Return the maximum coordinate on the Z axis
     * \return The maximum coordinate on the Z axis
     */
    float maxZ() const { return m_end.z; }

    /**
     * \brief Return the point whose coordinates are maximum
     * \return The point whose coordinates are maximum
     */
    const glm::vec3& max() const { return m_end; }

    /**
     * \brief Return the size on the X axis
     * \return The size on the X axis
     */
    float sizeX() const { return m_end.x - m_start.x; }

    /**
     * \brief Return the size on the Y axis
     * \return The size on the Y axis
     */
    float sizeY() const { return m_end.y - m_start.y; }

    /**
     * \brief Return the size on the Z axis
     * \return The size on the Z axis
     */
    float sizeZ() const { return m_end.z - m_start.z; }

    /**
     * \brief Return the center of the AABB
     * \return The center of the AABB
     */
    [[nodiscard]] glm::vec3 center() const { return (m_start + m_end) / 2.f; }

private:

    /**
     * \brief Minimum coordinates of the AABB
     */
    glm::vec3 m_start;

    /**
     * \brief Maximum coordinates of the AABB
     */
    glm::vec3 m_end;
};

bool rayAABBIntersection(const AABB& box, const Ray& ray);
