#pragma once

#include <string>

#include "FloatImage.h"

#ifdef QT_AVAILABLE
#include <QImage>
bool saveAsPNG(const FloatImage& image, const std::string& filename);
#endif

bool saveAsPPM(const FloatImage& image, const std::string& filename);
