#include "MathUtils.h"

Vec3 mapPoint(const Mat4& transformation, const Vec3& point)
{
	const Vec4 homogeneousPoint(point, 1.0);
	const auto homogeneousResult = transformation * homogeneousPoint;

	assert(homogeneousResult.w != 0.0);
	
	return Vec3(homogeneousResult) / homogeneousResult.w;
}

Vec3 mapVector(const Mat4& transformation, const Vec3& vector)
{
	const Vec4 homogeneousVector(vector, 0.0);
	const auto homogeneousResult = transformation * homogeneousVector;
	// Conversion from Vec4 to Vec3
	return homogeneousResult;
}
