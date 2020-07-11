#include "FloatImage.h"

#include <algorithm>

FloatImage::FloatImage(int width, int height) :
	m_width(width),
	m_height(height),
	m_data(m_width * m_height, { 0.f, 0.f, 0.f })
{
	assert(width > 0);
	assert(height > 0);
}

void FloatImage::atomicAdd(int row, int col, const glm::vec3& color)
{
	auto& pixel = at(row, col);
#pragma omp atomic
	pixel.x += color.x;
#pragma omp atomic
	pixel.y += color.y;
#pragma omp atomic
	pixel.z += color.z;
}

FloatImage& FloatImage::operator/=(float divisor)
{
	for (unsigned int i = 0; i < m_data.size(); i++)
	{
		m_data[i] /= divisor;
	}

	return *this;
}

void FloatImage::applyGammaCorrection()
{
	for (unsigned int i = 0; i < m_data.size(); i++)
	{
		m_data[i] = glm::sqrt(m_data[i]);
	}
}

FloatImage FloatImage::aggregateImages(const std::vector<FloatImage>& images)
{
	assert(!images.empty());
	
	int width = images.front().width();
	int height = images.front().height();

	// Compute minimum size among images
	for (unsigned int i = 0; i < images.size(); i++)
	{
		width = std::min(width, images[i].width());
		height = std::min(height, images[i].height());
	}

	FloatImage outputImage(width, height);
	
#pragma omp parallel for
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			for (const auto& image : images)
			{
				outputImage.at(i, j) += image.at(i, j);
			}
		}
	}
	
	return outputImage;
}
