#pragma once

#include <array>
#include <filesystem>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "AABB.h"
#include "Ray.h"
#include "Material.h"

struct MeshVertex
{
	Vec3 position;
	Vec3 normal;

	MeshVertex(const Vec3& position, const Vec3& normal) :
		position(position),
		normal(normal)
	{
		
	}
};

struct MeshFace
{
	std::array<int, 3> vertices;
	std::array<int, 3> normals;

	MeshFace(const std::array<int, 3>& vertices, const std::array<int, 3>& normals) :
		vertices(vertices),
		normals(normals)
	{

	}
};

class Mesh
{
public:
	Mesh();

	explicit Mesh(std::vector<MeshVertex> vertices, std::vector<MeshFace> faces);

	void setVertices(std::vector<MeshVertex> vertices);

	void setFaces(std::vector<MeshFace> faces);

	void setMaterial(std::shared_ptr<Material> material);

	void applyTransformation(const Mat4& transformation);

	int nbVertices() const;

	int nbFaces() const;

	const std::vector<MeshVertex>& vertices() const;

	const std::vector<MeshFace>& faces() const;

	const std::shared_ptr<Material>& material() const;

	const AABB& boundingBox() const;

	/**
	 * \brief Return the transformed vertex v (between 0 and 2) from a face
	 * \param face The index of the face of the mesh
	 * \param v The index of the vertex in the face
	 * \return The 3D coordinates of the vertex
	 */
	[[nodiscard]] Vec3 vertex(int face, int v) const;

	/**
	 * \brief Return the transformed normal at a vertex v (between 0 and 2) from a face
	 * \param face The index of the face of the mesh
	 * \param v The index of the vertex in the face
	 * \return The 3D coordinates of the normal
	 */
	[[nodiscard]] Vec3 normal(int face, int v) const;

	/**
	 * \brief Return the transformed normal interpolated with barycentric coordinates from a face
	 * \param face The index of the face of the mesh
	 * \param u The first barycentric coordinate
	 * \param v The second barycentric coordinate
	 * \return The 3D coordinates of the normal
	 */
	[[nodiscard]] Vec3 normal(int face, double u, double v) const;

private:
	/**
	 * \brief Update the internal AABB of the mesh.
	 *        Warning: run this when the mesh is updated.
	 */
	void updateBoundingBox();
	
	/**
	 * \brief Vertices and normals of the mesh
	 */
	std::vector<MeshVertex> m_vertices;
	
	/**
	 * \brief Faces of the mesh
	 */
	std::vector<MeshFace> m_faces;

	/**
	 * \brief Bounding box of the mesh for ray intersection acceleration
	 */
	AABB m_boundingBox;
	
	/**
	 * \brief Material of the mesh
	 */
	std::shared_ptr<Material> m_material;
};

bool loadMesh(const std::filesystem::path& filename, Mesh& mesh);
bool loadMesh(const std::filesystem::path& filename, std::vector<MeshVertex>& vertices, std::vector<MeshFace>& faces);

bool rayMeshIntersection(const Mesh& mesh,
	                     const Ray& ray,
						 double minT,
	                     HitRecord& hit);

bool rayMeshesIntersection(const std::vector<Mesh>& meshes,
	                       const Ray& ray,
						   double minT,
	                       HitRecord& hit);
