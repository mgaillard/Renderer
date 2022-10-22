#include "ImageConversion.h"

#include <algorithm>
#include <fstream>

#include "MathUtils.h"

#ifdef QT_AVAILABLE
bool saveAsPNG(const FloatImage& image, const std::string& filename)
{
	// Convert to image
	QImage outputImage(image.width(), image.height(), QImage::Format_RGB32);

#pragma omp parallel for
	for (int i = 0; i < image.height(); i++)
	{
		for (int j = 0; j < image.width(); j++)
		{
			// Conversion to RGB in [0; 255]
			const auto red = static_cast<uint8_t>(255.0 * std::clamp(image.at(i, j).x, 0.0, 1.0));
			const auto green = static_cast<uint8_t>(255.0 * std::clamp(image.at(i, j).y, 0.0, 1.0));
			const auto blue = static_cast<uint8_t>(255.0 * std::clamp(image.at(i, j).z, 0.0, 1.0));
			// Write the pixel in the image
			outputImage.setPixel(j, i, qRgb(red, green, blue));
		}
	}

	return outputImage.save(QString::fromStdString(filename));
}
#endif

bool saveAsPPM(const FloatImage& image, const std::string& filename)
{
	std::ofstream file(filename);

	if (!file.is_open())
	{
		return false;
	}
	
	file << "P3\n" << image.width() << ' ' << image.height() << "\n255\n";

	for (int i = 0; i < image.height(); i++)
	{
		for (int j = 0; j < image.width(); j++)
		{
			// Conversion to RGB in [0; 255]
			const auto red = static_cast<int>(255.0 * std::clamp(image.at(i, j).x, 0.0, 1.0));
			const auto green = static_cast<int>(255.0 * std::clamp(image.at(i, j).y, 0.0, 1.0));
			const auto blue = static_cast<int>(255.0 * std::clamp(image.at(i, j).z, 0.0, 1.0));

			file << red << ' ' << green << ' ' << blue << '\n';
		}
	}

	file.close();

	return false;
}
