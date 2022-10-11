#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, float focalLength, float aspectRatio) :
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

Ray Camera::generateRay(float x, float y) const
{
	const auto direction = m_lowerLeftCorner + x * m_horizontal + y * m_vertical - m_eye;

	return { m_eye, direction };
}

void Camera::updateCameraInternalVectors()
{
	constexpr float viewportHeight = 2.f;
	const float viewportWidth = m_aspectRatio * viewportHeight;

	const auto eyeToAt = glm::normalize(m_at - m_eye);

	m_horizontal = viewportWidth * glm::cross(eyeToAt, m_up);
	m_vertical = viewportHeight * m_up;
	
	m_lowerLeftCorner = m_eye
	                    - m_horizontal / 2.f
	                    - m_vertical / 2.f
	                    + m_focalLength * eyeToAt;
}
