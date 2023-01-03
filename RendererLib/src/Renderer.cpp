#include "Renderer.h"

#include <omp.h>

#include <iostream>
#include <sstream>

#include "FloatImage.h"

Renderer::Renderer(std::unique_ptr<Scene> scene, Random::SeedType randomSeed) :
	m_samplesPerPixels(128),
	m_maxDepth(32),
	m_minT(0.0001),
	m_randomSeed(randomSeed),
	m_scene(std::move(scene))
{
	
}

int Renderer::samplesPerPixels() const
{
	return m_samplesPerPixels;
}

int Renderer::maxDepth() const
{
	return m_maxDepth;
}

void Renderer::setSamplesPerPixels(int samplesPerPixels)
{
	m_samplesPerPixels = samplesPerPixels;
}

void Renderer::setMaxDepth(int maxDepth)
{
	m_maxDepth = maxDepth;
}

FloatImage Renderer::render(int width, int height) const
{
	auto floatImage = renderWithoutGammaCorrection(width, height);

	// Multi-sampling and Gamma correction, with gamma=2.0
	floatImage /= static_cast<double>(m_samplesPerPixels);
	floatImage.applyGammaCorrection();

	return floatImage;
}

FloatImage Renderer::renderWithoutGammaCorrection(int width, int height) const
{
	const auto nbThreads = omp_get_max_threads();

	// Random number generators per thread, for reproducibility
	auto randomGenerators = Random::createMultiple(nbThreads, m_randomSeed);

	// 2D array of colors per thread
	std::vector<FloatImage> floatImages(nbThreads, FloatImage(width, height));

	// The progress is the number of horizontal scan lines times the number of samples
	const long totalProgress = m_samplesPerPixels * height;
	long progress = 0;

	#pragma omp parallel for
	for (int s = 0; s < m_samplesPerPixels; s++)
	{
		const auto currentThread = omp_get_thread_num();

		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				auto& randomGenerator = randomGenerators[currentThread];

				// Compute normalized coordinates (randomized), y is reversed in the image
				const auto x = (static_cast<double>(j) + randomGenerator.randomNumber()) / (width - 1);
				const auto y = (static_cast<double>(height - 1 - i) + randomGenerator.randomNumber()) / (height - 1);

				// Generate a ray from the camera
				const auto ray = m_scene->camera()->generateRay(x, y);

				// Add colors for each sample per pixel
				const auto rayColor = computeRayColor(ray, m_maxDepth, randomGenerator);

				floatImages[currentThread].at(i, j) += rayColor;
			}

			// Update progress after each full line
			#pragma omp atomic
			progress++;

			// The main thread is responsible for displaying the progress
			if (currentThread == 0)
			{
				// TODO: display the progress each second, and the ETA
				// Use string stream to prevent some of the concurrency issues when displaying
				std::stringstream progressStream;
				progressStream << "Progress: " << static_cast<double>(progress) / static_cast<double>(totalProgress) << std::endl;
				std::cout << progressStream.str();
			}
		}
	}

	// Aggregate the images from each CPU core
	return FloatImage::aggregateImages(floatImages);
}

Vec3 Renderer::computeRayColor(const Ray& ray, int depth, Random& randomGenerator) const
{	
	Vec3 color(0.0, 0.0, 0.0);

	// Ray bounce limit is not exceeded yet
	if (depth > 0)
	{
		// Compute intersection with meshes
		HitRecord hit;
		if (rayMeshesIntersection(m_scene->meshes(), ray, m_minT, hit))
		{
			Vec3 attenuation;
			Ray scatteredRay;
			
			if (hit.material->scatter(hit, randomGenerator, attenuation, scatteredRay))
			{
				return attenuation * computeRayColor(scatteredRay, depth - 1, randomGenerator);
			}
		}
		else
		{
			// No intersection, display the background color
			Vec3 unitDirection = glm::normalize(ray.direction());
			const auto t = 0.5 * (unitDirection.y + 1.0);
			return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
		}
	}

	return color;
}
