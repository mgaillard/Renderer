#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const Vec3& eye, const Vec3& at, const Vec3& up, double focalLength, double aspectRatio) :
	m_eye(eye),
	m_at(at),
	m_up(glm::normalize(up)),
	m_focalLength(focalLength),
	m_aspectRatio(aspectRatio),
	m_horizontal(),
	m_vertical(),
	m_lowerLeftCorner()
{
	updateCameraInternalVectors();
}

Ray Camera::generateRay(double x, double y) const
{
	const auto direction = m_lowerLeftCorner + x * m_horizontal + y * m_vertical - m_eye;

	return { m_eye, direction };
}

void Camera::updateCameraInternalVectors()
{
	constexpr double viewportHeight = 2.0;
	const double viewportWidth = m_aspectRatio * viewportHeight;

	const auto eyeToAt = glm::normalize(m_at - m_eye);

	m_horizontal = viewportWidth * glm::cross(eyeToAt, m_up);
	m_vertical = viewportHeight * m_up;
	
	m_lowerLeftCorner = m_eye
	                    - m_horizontal / 2.0
	                    - m_vertical / 2.0
	                    + m_focalLength * eyeToAt;
}
