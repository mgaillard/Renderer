#pragma once

#include <vector>

#include <glm/glm.hpp>

class FloatImage
{
public:
	FloatImage(int width, int height);

	int width() const { return m_width; }
	int height() const { return m_height; }

	glm::vec3& at(int row, int col)
	{
		assert(row >= 0 && row < m_height);
		assert(col >= 0 && col < m_width);

		return m_data[row * m_width + col];
	}
	
	const glm::vec3& at(int row, int col) const
	{
		assert(row >= 0 && row < m_height);
		assert(col >= 0 && col < m_width);

		return m_data[row * m_width + col];
	}

	void atomicAdd(int row, int col, const glm::vec3& color);

	/**
	 * \brief Divide all pixels in the image by a float number.
	 *        Useful when using multi-sampling.
	 * \param divisor The divisor
	 * \return 
	 */
	FloatImage& operator/=(float divisor);

	/**
	 * \brief Apply gamma correction on the image, with gamma = 2.0
	 */
	void applyGammaCorrection();

	/**
	 * \brief Aggregate multiple images into one image.
	 *        Add pixels of all images into the output image.
	 * \param images A list of images to aggregate into one
	 * \return The aggregated image
	 */
	static FloatImage aggregateImages(const std::vector<FloatImage>& images);

private:
	int m_width;
	int m_height;
	
	std::vector<glm::vec3> m_data;
};

