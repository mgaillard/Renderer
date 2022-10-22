#pragma once

#include <random>

#include "Scene.h"
#include "FloatImage.h"
#include "Random.h"

class Renderer
{
public:
	explicit Renderer(std::unique_ptr<Scene> scene,
		              Random::SeedType randomSeed = Random::GeneratorType::default_seed);

	[[nodiscard]] FloatImage render(int width, int height) const;

	[[nodiscard]] FloatImage renderWithoutGammaCorrection(int width, int height) const;

private:
	[[nodiscard]] Vec3 computeRayColor(const Ray& ray, int depth, Random& randomGenerator) const;

	const int m_samplesPerPixels = 128;
	const int m_maxDepth = 32;
	const double m_minT;
	
	Random::SeedType m_randomSeed;
	
	std::unique_ptr<Scene> m_scene;
};
