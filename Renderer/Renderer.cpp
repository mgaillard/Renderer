#include "Renderer.h"

#include <omp.h>

#include <iostream>

#include "FloatImage.h"
#include "Random.h"
#include "MathUtils.h"

Renderer::Renderer(std::unique_ptr<Scene> scene) :
	m_minT(0.0001f),
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
				const auto x = (float(j) + Random::randomNumber()) / (width - 1);
				const auto y = (float(height - 1 - i) + Random::randomNumber()) / (height - 1);

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

		std::cout << "Progress: " << float(progress) / float(totalProgress) << std::endl;
	}

	// Aggregate the images from each CPU core
	auto floatImage = FloatImage::aggregateImages(floatImages);

	// Multi-sampling and Gamma correction, with gamma=2.0
	floatImage /= float(m_samplesPerPixels);
	floatImage.applyGammaCorrection();

	return floatImage;
}

glm::vec3 Renderer::computeRayColor(const Ray& ray, int depth) const
{	
	glm::vec3 color(0.f, 0.f, 0.f);

	// Ray bounce limit is not exceeded yet
	if (depth > 0)
	{
		// Compute intersection with meshes
		HitRecord hit;
		if (rayMeshesIntersection(m_scene->meshes(), ray, m_minT, hit))
		{
			glm::vec3 attenuation;
			Ray scatteredRay;
			
			if (hit.material->scatter(hit, attenuation, scatteredRay))
			{
				return attenuation * computeRayColor(scatteredRay, depth - 1);
			}
		}
		else
		{
			// No intersection, display the background color
			glm::vec3 unitDirection = glm::normalize(ray.direction());
			const auto t = 0.5f * (unitDirection.y + 1.f);
			return (1.f - t) * glm::vec3(1.f, 1.f, 1.f) + t * glm::vec3(0.5f, 0.7f, 1.f);
		}
	}

	return color;
}
