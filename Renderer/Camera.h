#pragma once

#include <glm/glm.hpp>

#include "Ray.h"

class Camera
{
public:
	/**
	 * \brief Create a camera
	 * \param eye The position of the eye
	 * \param at The position of the at, where the camera is looking
	 * \param up The up vector
	 * \param focalLength Focal length of the camera
	 * \param aspectRatio Aspect ratio of the camera
	 */
	Camera(const glm::vec3& eye,
		   const glm::vec3& at,
		   const glm::vec3& up,
		   float focalLength,
		   float aspectRatio);

	/**
	 * \brief Generate a ray going through this camera
	 * \param x Coordinates of the ray on X in [0; 1]
	 * \param y Coordinates of the ray on Y in [0; 1]
	 * \return A ray
	 */
	[[nodiscard]] Ray generateRay(float x, float y) const;

private:
	glm::vec3 m_eye;
	glm::vec3 m_at;
	glm::vec3 m_up;

	float m_focalLength;
	float m_aspectRatio;

	// Following are internal members caching vectors for faster ray generation
	glm::vec3 m_horizontal;
	glm::vec3 m_vertical;
	glm::vec3 m_lowerLeftCorner;

	/**
	 * \brief Update the horizontal, vertical and lowerLeftCorner vectors.
	 *        Warning: should be called whenever camera parameters are modified.
	 */
	void updateCameraInternalVectors();
};

