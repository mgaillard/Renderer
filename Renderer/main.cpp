#include <chrono>
#include <iostream>

#include "ImageConversion.h"
#include "ReferenceScene.h"
#include "Renderer.h"

int main()
{
    constexpr int width = 1280 / 4;
    constexpr int height = 720 / 4;
    constexpr int samplesPerPixels = 128;

    auto scene = createReferenceScene(width, height, "../models/");

    Renderer renderer(std::move(scene));
    renderer.setSamplesPerPixels(samplesPerPixels);
    const auto start = std::chrono::system_clock::now();
    const auto floatImage = renderer.render(width, height);
    const auto end = std::chrono::system_clock::now();
    const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Rendering time: " << elapsedTime << " ms\n";
    saveAsPPM(floatImage, "output.ppm");

    return 0;
}
