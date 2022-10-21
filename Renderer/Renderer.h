#pragma once

#include <random>

#include "Scene.h"
#include "FloatImage.h"

class Renderer
{
public:
	explicit Renderer(std::unique_ptr<Scene> scene);

	[[nodiscard]] FloatImage render(int width, int height) const;

private:
	[[nodiscard]] Vec3 computeRayColor(const Ray& ray, int depth) const;

	const int m_samplesPerPixels = 128;
	const int m_maxDepth = 32;
	const double m_minT;
	
	std::mt19937 m_randomGenerator;
	
	std::unique_ptr<Scene> m_scene;
};
