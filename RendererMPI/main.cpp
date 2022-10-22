#include <chrono>
#include <iostream>
#include <mpi.h>

#include <glm/gtx/transform.hpp>

#include "ImageConversion.h"
#include "Lambertian.h"
#include "Renderer.h"
#include "Scene.h"

Renderer setupRenderer(int width, int height, Random::SeedType seed)
{
    // Setup camera
    constexpr Vec3 eye(0, 10, 25);
    constexpr Vec3 at(0, 0, 0);
    constexpr Vec3 right(1, 0, 0);
    const Vec3 up = glm::normalize(glm::cross(right, at - eye));
    constexpr double focalLength = 2.0;
    const double aspectRatio = static_cast<double>(width) / static_cast<double>(height);
    auto camera = std::make_unique<Camera>(eye, at, up, focalLength, aspectRatio);

    // Setup the available materials
    const auto diffuseGrey = std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));

    // Load meshes
    std::vector<Mesh> meshes;

    Mesh floor;
    loadMesh("../models/floor.obj", floor);
    auto floorTransformation = glm::scale(Vec3(40.0, 1.0, 40.0));
    floorTransformation = glm::translate(floorTransformation, Vec3(-0.5, 0.0, -0.5));
    floor.applyTransformation(floorTransformation);
    floor.setMaterial(diffuseGrey);
    meshes.push_back(floor);

    // Setup scene
    auto scene = std::make_unique<Scene>(std::move(camera), std::move(meshes));

    Renderer renderer(std::move(scene), seed);

    return renderer;
}

FloatImage aggregateImagesMPI(const FloatImage& images)
{
    FloatImage outputImage(images.width(), images.height());

    // The total number of double values in the image is the number of pixels times 3 
    const auto nbPixels = images.width() * images.height() * 3;

    MPI_Reduce(&images.at(0, 0), &outputImage.at(0, 0), nbPixels, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

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

    constexpr int width = 1280;
    constexpr int height = 720;
    constexpr int samplesPerPixels = 128;
    // Setup the scene and the renderer (done on each node)
    const auto renderer = setupRenderer(width, height, nodeSeed);
    const auto floatImage = renderer.renderWithoutGammaCorrection(width, height);

    // Aggregate the random images with MPI
    auto outputImage = aggregateImagesMPI(floatImage);

    // Save the image from the master node
    if (world_rank == 0)
    {
        // Multi-sampling and Gamma correction, with gamma=2.0
        outputImage /= static_cast<double>(world_size * samplesPerPixels);
        outputImage.applyGammaCorrection();

        saveAsPPM(outputImage, "output.ppm");
    }
    
    MPI_Finalize();

    return 0;
}
