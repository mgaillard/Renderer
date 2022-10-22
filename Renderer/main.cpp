#include <glm/gtx/transform.hpp>

#include <chrono>
#include <iostream>

#include "Dielectric.h"
#include "ImageConversion.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Renderer.h"
#include "Random.h"
#include "Scene.h"
#include "Types.h"

void addRandomSphereWithoutCollisions(Random& randomGenerator, std::vector<Mesh>& meshes)
{
    

    // Random position over the floor
    const Vec3 position(
        randomGenerator.randomNumberInterval(-18.0, 18.0),
        1.0,
        randomGenerator.randomNumberInterval(-18.0, 18.0)
    );

	// Random color
    const Vec3 color(
        randomGenerator.randomNumberUnit(),
        randomGenerator.randomNumberUnit(),
        randomGenerator.randomNumberUnit()
    );

    bool discard = false;
	// Check for collisions
	for (const auto& mesh : meshes)
	{
		for (const auto& v : mesh.vertices())
		{
			if (glm::distance(position, v.position) < 1.0)
			{
                discard = true;
			}
		}
	}
	
	if (!discard)
	{
        auto material = std::make_shared<Lambertian>(color);

        Mesh sphere;
        loadMesh("../models/simple_sphere.obj", sphere);
        const auto sphereTransformation = glm::translate(position);
        sphere.applyTransformation(sphereTransformation);
        sphere.setMaterial(std::move(material));
        meshes.push_back(sphere);
	}
}

int main()
{
    constexpr int width = 1280 / 4;
    constexpr int height = 720 / 4;

	// Setup camera
    constexpr Vec3 eye(0, 10, 25);
    constexpr Vec3 at(0, 0, 0);
    constexpr Vec3 right(1, 0, 0);
    const Vec3 up = glm::normalize(glm::cross(right, at - eye));
    constexpr double focalLength = 2.0;
    constexpr double aspectRatio = static_cast<double>(width) / static_cast<double>(height);
    auto camera = std::make_unique<Camera>(eye, at, up, focalLength, aspectRatio);

	// Setup the available materials
    const auto diffuseGrey = std::make_shared<Lambertian>(Vec3(0.5, 0.5, 0.5));
    const auto diffuseGreen = std::make_shared<Lambertian>(Vec3(0.1, 0.8, 0.1));
    const auto diffuseRed = std::make_shared<Lambertian>(Vec3(0.8, 0.1, 0.1));
    const auto metal = std::make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.15);
    const auto glass = std::make_shared<Dielectric>(1.5);
	
	// Load meshes
    std::vector<Mesh> meshes;

    Mesh floor;
    loadMesh("../models/floor.obj", floor);
    auto floorTransformation = glm::scale(Vec3(40.0, 1.0, 40.0));
    floorTransformation = glm::translate(floorTransformation, Vec3(-0.5, 0.0, -0.5));
    floor.applyTransformation(floorTransformation);
    floor.setMaterial(diffuseGrey);
    meshes.push_back(floor);

    Mesh sphereDiffuseRed;
    loadMesh("../models/simple_sphere.obj", sphereDiffuseRed);
    auto sphereRedTransformation = glm::scale(Vec3(1.5, 1.5, 1.5));
    sphereRedTransformation = glm::translate(sphereRedTransformation, Vec3(1.0, 1.0, 3.5));
    sphereDiffuseRed.applyTransformation(sphereRedTransformation);
    sphereDiffuseRed.setMaterial(diffuseRed);
    meshes.push_back(sphereDiffuseRed);

    Mesh sphereMetal;
    loadMesh("../models/smooth_sphere.obj", sphereMetal);
    auto sphereMetalTransformation = glm::scale(Vec3(4.0, 4.0, 4.0));
    sphereMetalTransformation = glm::translate(sphereMetalTransformation, Vec3(1.5, 1.0, 0.0));
    sphereMetal.applyTransformation(sphereMetalTransformation);
    sphereMetal.setMaterial(metal);
    meshes.push_back(sphereMetal);

    Mesh sphereDiffuseGreen;
    loadMesh("../models/simple_sphere.obj", sphereDiffuseGreen);
    auto sphereDiffuseGreenTransformation = glm::scale(Vec3(3.0, 3.0, 3.0));
    sphereDiffuseGreenTransformation = glm::translate(sphereDiffuseGreenTransformation,Vec3(-2.0, 1.0, 0.0));
    sphereDiffuseGreen.applyTransformation(sphereDiffuseGreenTransformation);
    sphereDiffuseGreen.setMaterial(diffuseGreen);
    meshes.push_back(sphereDiffuseGreen);

    Mesh cow;
    loadMesh("../models/cow.obj", cow);
    const auto cowTransformation = glm::translate(Vec3(-2.0, 3.6, 6.0));
    cow.applyTransformation(cowTransformation);
    cow.setMaterial(glass);
    meshes.push_back(cow);

    Random randomSphereGenerator;
	for (int i = 0; i < 50; i++)
	{
        addRandomSphereWithoutCollisions(randomSphereGenerator, meshes);
	}

	// Setup scene
    auto scene = std::make_unique<Scene>(std::move(camera), std::move(meshes));

    const Renderer renderer(std::move(scene));
    const auto start = std::chrono::system_clock::now();
    const auto floatImage = renderer.render(width, height);
    const auto end = std::chrono::system_clock::now();
    const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Rendering time: " << elapsedTime << " ms\n";
    saveAsPPM(floatImage, "output.ppm");

    return 0;
}
