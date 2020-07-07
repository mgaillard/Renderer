#include "Renderer.h"

#include <omp.h>

#include <QtDebug>

#include "Random.h"
#include "MathUtils.h"

Renderer::Renderer(std::unique_ptr<Scene> scene) :
	m_minT(0.0001f),
	m_scene(std::move(scene))
{
	
}

QImage Renderer::render(int width, int height) const
{
	// 2D array of colors
	std::vector<std::vector<glm::vec3>> color(height, std::vector<glm::vec3>(width, {0, 0, 0}));

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
				color[i][j] += computeRayColor(ray, m_maxDepth);
			}
		}

		// Show progress after each full image
#pragma omp atomic
		progress++;

		qDebug() << "Progress:" << float(progress) / float(totalProgress);
	}

	// Convert to image
	QImage image(width, height, QImage::Format_RGB32);

#pragma omp parallel for
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// Multi-sampling and Gamma correction, with gamma=2.0
			color[i][j] = glm::sqrt(color[i][j] / float(m_samplesPerPixels));
			// Conversion to RGB in [0; 255]
			const auto red = static_cast<uint8_t>(255.0f * clamp(color[i][j].x, 0.f, 1.f));
			const auto green = static_cast<uint8_t>(255.0f * clamp(color[i][j].y, 0.f, 1.f));
			const auto blue = static_cast<uint8_t>(255.0f * clamp(color[i][j].z, 0.f, 1.f));
			// Write the pixel in the image
			image.setPixel(j, i, qRgb(red, green, blue));
		}
	}

	return image;
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
