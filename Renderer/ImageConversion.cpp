#include "ImageConversion.h"

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
			const auto red = static_cast<uint8_t>(255.0f * clamp(image.at(i, j).x, 0.f, 1.f));
			const auto green = static_cast<uint8_t>(255.0f * clamp(image.at(i, j).y, 0.f, 1.f));
			const auto blue = static_cast<uint8_t>(255.0f * clamp(image.at(i, j).z, 0.f, 1.f));
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
			const auto red = int(255.0f * clamp(image.at(i, j).x, 0.f, 1.f));
			const auto green = int(255.0f * clamp(image.at(i, j).y, 0.f, 1.f));
			const auto blue = int(255.0f * clamp(image.at(i, j).z, 0.f, 1.f));

			file << red << ' ' << green << ' ' << blue << '\n';
		}
	}

	file.close();

	return false;
}
