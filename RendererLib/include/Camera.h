#pragma once

#include "Ray.h"
#include "Types.h"

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
	Camera(const Vec3& eye,
		   const Vec3& at,
		   const Vec3& up,
		   double focalLength,
		   double aspectRatio);

	/**
	 * \brief Generate a ray going through this camera
	 * \param x Coordinates of the ray on X in [0; 1]
	 * \param y Coordinates of the ray on Y in [0; 1]
	 * \return A ray
	 */
	[[nodiscard]] Ray generateRay(double x, double y) const;

private:
	Vec3 m_eye;
	Vec3 m_at;
	Vec3 m_up;

	double m_focalLength;
	double m_aspectRatio;

	// Following are internal members caching vectors for faster ray generation
	Vec3 m_horizontal;
	Vec3 m_vertical;
	Vec3 m_lowerLeftCorner;

	/**
	 * \brief Update the horizontal, vertical and lowerLeftCorner vectors.
	 *        Warning: should be called whenever camera parameters are modified.
	 */
	void updateCameraInternalVectors();
};

