#include "Scene.h"

Scene::Scene(std::unique_ptr<Camera> camera, std::vector<Mesh> meshes) :
	m_camera(std::move(camera)),
	m_meshes(std::move(meshes))
{
	
}

void Scene::setCamera(std::unique_ptr<Camera> camera)
{
	m_camera = std::move(camera);
}

void Scene::setMesh(const Mesh& mesh)
{
	m_meshes.clear();
	m_meshes.push_back(mesh);
}

void Scene::setMeshes(std::vector<Mesh> meshes)
{
	m_meshes = std::move(meshes);
}

const std::unique_ptr<Camera>& Scene::camera() const
{
	return m_camera;
}

const std::vector<Mesh>& Scene::meshes() const
{
	return m_meshes;
}
