#include <QtCore/QCoreApplication>

#include <glm/gtx/transform.hpp>


#include "Dielectric.h"
#include "Lambertian.h"
#include "Metal.h"
#include "Scene.h"
#include "Renderer.h"

int main(int argc, char *argv[])
{
    const int width = 1280;
    const int height = 720;

	// Setup camera
    const glm::vec3 eye(0, 10, 25);
    const glm::vec3 at(0, 0, 0);
    const glm::vec3 right(1, 0, 0);
    const glm::vec3 up = glm::normalize(glm::cross(right, at - eye));
    const float focalLength = 1.0f;
    const float aspectRatio = float(width) / float(height);
    auto camera = std::make_unique<Camera>(eye, at, up, focalLength, aspectRatio);

	// Setup the available materials
    const auto diffuseGrey = std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f));
    const auto diffuseGreen = std::make_shared<Lambertian>(glm::vec3(0.1f, 0.8f, 0.1f));
    const auto diffuseRed = std::make_shared<Lambertian>(glm::vec3(0.8f, 0.1f, 0.1f));
    const auto metal = std::make_shared<Metal>(glm::vec3(7.f, 6.f, 5.f), 0.15f);
    const auto glass = std::make_shared<Dielectric>(1.5f);
	
	// Load meshes
    std::vector<Mesh> meshes;

    Mesh floor;
    loadMesh("../models/floor.obj", floor);
    auto floorTransformation = glm::scale(glm::vec3(25.f, 1.f, 25.f));
    floorTransformation = glm::translate(floorTransformation, glm::vec3(-0.5f, 0.f, -0.5f));
    floor.applyTransformation(floorTransformation);
    floor.setMaterial(diffuseGrey);
    meshes.push_back(floor);

    Mesh sphereDiffuseRed;
    loadMesh("../models/simple_sphere.obj", sphereDiffuseRed);
    auto sphereRedTransformation = glm::scale(glm::vec3(1.5f, 1.5f, 1.5f));
    sphereRedTransformation = glm::translate(sphereRedTransformation, glm::vec3(1.f, 1.f, 3.5f));
    sphereDiffuseRed.applyTransformation(sphereRedTransformation);
    sphereDiffuseRed.setMaterial(diffuseRed);
    meshes.push_back(sphereDiffuseRed);

    Mesh sphereMetal;
    loadMesh("../models/smooth_sphere.obj", sphereMetal);
    auto sphereMetalTransformation = glm::scale(glm::vec3(4.f, 4.f, 4.f));
    sphereMetalTransformation = glm::translate(sphereMetalTransformation, glm::vec3(1.5f, 1.0f, 0.f));
    sphereMetal.applyTransformation(sphereMetalTransformation);
    sphereMetal.setMaterial(metal);
    meshes.push_back(sphereMetal);

    Mesh sphereDiffuseGreen;
    loadMesh("../models/simple_sphere.obj", sphereDiffuseGreen);
    auto sphereDiffuseGreenTransformation = glm::scale(glm::vec3(3.f, 3.f, 3.f));
    sphereDiffuseGreenTransformation = glm::translate(sphereDiffuseGreenTransformation,glm::vec3(-2.f, 1.0f, 0.f));
    sphereDiffuseGreen.applyTransformation(sphereDiffuseGreenTransformation);
    sphereDiffuseGreen.setMaterial(diffuseGreen);
    meshes.push_back(sphereDiffuseGreen);

    Mesh cow;
    loadMesh("../models/cow.obj", cow);
    const auto cowTransformation = glm::translate(glm::vec3(-2.f, 3.6f, 6.f));
    cow.applyTransformation(cowTransformation);
    cow.setMaterial(glass);
    meshes.push_back(cow);

	// Setup scene
    auto scene = std::make_unique<Scene>(std::move(camera), std::move(meshes));

    const Renderer renderer(std::move(scene));
    const auto image = renderer.render(width, height);
    image.save("output.png");

    return 0;
}