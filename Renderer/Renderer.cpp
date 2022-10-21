#include "Renderer.h"

#include <omp.h>

#include <iostream>
#include <sstream>

#include "FloatImage.h"
#include "Random.h"

Renderer::Renderer(std::unique_ptr<Scene> scene) :
	m_minT(0.0001),
	m_scene(std::move(scene))
{
	
}

FloatImage Renderer::render(int width, int height) const
{	
	// 2D array of colors for each thread on the computer
	std::vector<FloatImage> floatImages(omp_get_max_threads(), FloatImage(width, height));

	long progress = 0;
	const long totalProgress = m_samplesPerPixels;

#pragma omp parallel for
	for (int s = 0; s < m_samplesPerPixels; s++)
	{
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				// Compute normalized coordinates, y is reversed in the image
				const auto x = (static_cast<double>(j) + Random::randomNumber()) / (width - 1);
				const auto y = (static_cast<double>(height - 1 - i) + Random::randomNumber()) / (height - 1);

				// Generate a ray from the camera
				const auto ray = m_scene->camera()->generateRay(x, y);

				// Add colors for each sample per pixel
				const auto rayColor = computeRayColor(ray, m_maxDepth);

				const auto currentThread = omp_get_thread_num();
				floatImages[currentThread].at(i, j) += rayColor;
			}
		}

		// Show progress after each full image
#pragma omp atomic
		progress++;

		// Use string stream to prevent some of the concurrency issues when displaying
		std::stringstream progressStream;
		progressStream << "Progress: " << static_cast<double>(progress) / static_cast<double>(totalProgress) << std::endl;
		std::cout << progressStream.str();
	}

	// Aggregate the images from each CPU core
	auto floatImage = FloatImage::aggregateImages(floatImages);

	// Multi-sampling and Gamma correction, with gamma=2.0
	floatImage /= static_cast<double>(m_samplesPerPixels);
	floatImage.applyGammaCorrection();

	return floatImage;
}

Vec3 Renderer::computeRayColor(const Ray& ray, int depth) const
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
			
			if (hit.material->scatter(hit, attenuation, scatteredRay))
			{
				return attenuation * computeRayColor(scatteredRay, depth - 1);
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
