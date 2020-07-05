#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3& eye, const glm::vec3& at, const glm::vec3& up, float focalLength, float aspectRatio) :
	m_eye(eye),
	m_at(at),
	m_up(up),
	m_focalLength(focalLength),
	m_aspectRatio(aspectRatio)
{
	
}

Ray Camera::generateRay(float x, float y) const
{
	const float viewportHeight = 2.0;
	const float viewportWidth = m_aspectRatio * viewportHeight;
	
	const glm::vec3 eyeToAt = viewportWidth * (m_at - m_eye);
	const glm::vec3 right = viewportHeight * glm::normalize(glm::cross(eyeToAt, m_up));
	const glm::vec3 lowerLeftCorner = m_eye
	                                - m_up / 2.f
	                                - right / 2.f
	                                + m_focalLength * glm::normalize(eyeToAt);

	return { m_eye, lowerLeftCorner + x * right + y * m_up - m_eye};
}
