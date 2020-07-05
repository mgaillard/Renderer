#pragma once

#include <memory>
#include <vector>

#include <QImage>

#include "Camera.h"
#include "Mesh.h"

class Scene
{
public:
	Scene() = default;
	Scene(std::unique_ptr<Camera> camera, std::vector<Mesh> meshes);

	void setCamera(std::unique_ptr<Camera> camera);
	void setMesh(const Mesh& mesh);
	void setMeshes(std::vector<Mesh> meshes);

	const std::unique_ptr<Camera>& camera() const;
	const std::vector<Mesh>& meshes() const;

private:
	std::unique_ptr<Camera> m_camera;
	std::vector<Mesh> m_meshes;
};
