#include <chrono>
#include <iostream>
#include <mpi.h>

#include <glm/gtx/transform.hpp>

#include "ImageConversion.h"
#include "ReferenceScene.h"
#include "Renderer.h"

FloatImage aggregateImagesMPI(const FloatImage& images)
{
    FloatImage outputImage(images.width(), images.height());

    // The total number of double values in the image is the number of pixels times 3 
    const auto nbPixels = images.width() * images.height() * 3;

    MPI_Reduce(images.data(), outputImage.data(), nbPixels, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    return outputImage;
}

int main()
{
    MPI_Init(NULL, NULL);

    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Generate a unique random seed per compute node
    const auto seed = Random::GeneratorType::default_seed;
    const auto nodeSeed = 449251 * seed + 26722 * static_cast<Random::SeedType>(world_rank);

    // Setup the scene and the renderer (done on each node)
    constexpr int width = 1280 / 4;
    constexpr int height = 720 / 4;
    constexpr int totalSamplesPerPixels = 128;
    const int samplesPerPixels = 128 / world_size;
    auto scene = createReferenceScene(width, height, "../models/");
    Renderer renderer(std::move(scene), nodeSeed);
    renderer.setSamplesPerPixels(samplesPerPixels);

    // Measure rendering time
    const double start_time = MPI_Wtime();
    // Run the rendering code
    const auto floatImage = renderer.renderWithoutGammaCorrection(width, height);
    // Aggregate the random images with MPI
    auto outputImage = aggregateImagesMPI(floatImage);
    const double end_time = MPI_Wtime();

    // Save the image from the master node
    if (world_rank == 0)
    {
        // Multi-sampling and Gamma correction, with gamma=2.0
        outputImage /= static_cast<double>(totalSamplesPerPixels);
        outputImage.applyGammaCorrection();

        saveAsPPM(outputImage, "output.ppm");

        std::cout << "Calculated on " << world_size << " nodes in "
                  << end_time - start_time << " s" << std::endl;
    }
    
    MPI_Finalize();

    return 0;
}
