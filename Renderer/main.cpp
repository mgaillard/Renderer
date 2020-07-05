#include <QtCore/QCoreApplication>

#include <glm/gtx/transform.hpp>


#include "Lambertian.h"
#include "Scene.h"
#include "Renderer.h"

int main(int argc, char *argv[])
{
    const int width = 640;
    const int height = 360;

	// Setup camera
    const glm::vec3 eye(0, 15, 25);
    const glm::vec3 at(0, 0, 0);
    const glm::vec3 right(1, 0, 0);
    const glm::vec3 up = glm::normalize(glm::cross(right, at - eye));
    const float focalLength = 4.0f;
    const float aspectRatio = float(width) / float(height);
    auto camera = std::make_unique<Camera>(eye, at, up, focalLength, aspectRatio);

	// Setup the available materials
    const auto lambertian = std::make_shared<Lambertian>(glm::vec3(0.5f, 0.5f, 0.5f));
	
	// Load meshes
    std::vector<Mesh> meshes(2);
    loadMesh("../models/simple_sphere.obj", meshes[0]);
    loadMesh("../models/floor.obj", meshes[1]);

    // const auto cowTransformation = glm::translate(glm::vec3(0.f, 3.6f, 0.f));
    // meshes[0].setTransformation(cowTransformation);
    
    const auto sphereTransformation = glm::translate(glm::vec3(0.f, 1.0f, 0.f));
    meshes[0].setTransformation(sphereTransformation);
    meshes[0].setMaterial(lambertian);
	
    auto floorTransformation = glm::scale(glm::vec3(25.f, 1.f, 25.f));
    floorTransformation = glm::translate(floorTransformation, glm::vec3(-0.5f, 0.f, -0.5f));
    meshes[1].setTransformation(floorTransformation);
    meshes[1].setMaterial(lambertian);

	// Setup scene
    auto scene = std::make_unique<Scene>(std::move(camera), std::move(meshes));

    Renderer renderer(std::move(scene));
    const auto image = renderer.render(width, height);
    image.save("output.png");

    return 0;
}
