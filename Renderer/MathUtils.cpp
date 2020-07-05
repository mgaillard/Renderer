#include "MathUtils.h"

glm::vec3 mapPoint(const glm::mat4& transformation, const glm::vec3& point)
{
	const glm::vec4 homogeneousPoint(point, 1.f);
	const glm::vec4 homogeneousResult = transformation * homogeneousPoint;

	assert(homogeneousResult.w != 0.f);
	
	return glm::vec3(homogeneousResult) / homogeneousResult.w;
}

glm::vec3 mapVector(const glm::mat4& transformation, const glm::vec3& vector)
{
	const glm::vec4 homogeneousVector(vector, 0.f);
	const glm::vec4 homogeneousResult = transformation * homogeneousVector;
	return glm::vec3(homogeneousResult);
}
