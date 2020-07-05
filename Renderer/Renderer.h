#pragma once

#include <random>

#include "Scene.h"

class Renderer
{
public:
	explicit Renderer(std::unique_ptr<Scene> scene);

	QImage render(int width, int height) const;

private:

	glm::vec3 computeRayColor(const Ray& ray, int depth) const;

	const int m_samplesPerPixels = 32;
	const int m_maxDepth = 32;
	const float m_minT;
	
	std::mt19937 m_randomGenerator;
	
	std::unique_ptr<Scene> m_scene;
};

