#include "Mesh.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtx/intersect.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "MathUtils.h"

Mesh::Mesh() :
	m_boundingBox({0.0, 0.0, 0.0}, { 0.0, 0.0, 0.0 }),
	m_material(nullptr)
{
}

Mesh::Mesh(std::vector<MeshVertex> vertices, std::vector<MeshFace> faces) :
	m_vertices(std::move(vertices)),
	m_faces(std::move(faces)),
	m_boundingBox({ 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0 }),
	m_material(nullptr)
{
	updateBoundingBox();
}

void Mesh::setVertices(std::vector<MeshVertex> vertices)
{
	m_vertices = std::move(vertices);

	updateBoundingBox();
}

void Mesh::setFaces(std::vector<MeshFace> faces)
{
	m_faces = std::move(faces);
}

void Mesh::applyTransformation(const Mat4& transformation)
{
	const auto normalTransformation = glm::inverseTranspose(transformation);
	
	// Transform all vertices and normals
	for (unsigned int i = 0; i < m_vertices.size(); i++)
	{
		m_vertices[i].position = mapPoint(transformation, m_vertices[i].position);
		m_vertices[i].normal = mapVector(normalTransformation, m_vertices[i].normal);
	}

	updateBoundingBox();
}

int Mesh::nbVertices() const
{
	return static_cast<int>(m_vertices.size());
}

int Mesh::nbFaces() const
{
	return static_cast<int>(m_faces.size());
}

void Mesh::setMaterial(std::shared_ptr<Material> material)
{
	m_material = std::move(material);
}

const std::vector<MeshVertex>& Mesh::vertices() const
{
	return m_vertices;
}

const std::vector<MeshFace>& Mesh::faces() const
{
	return m_faces;
}

const std::shared_ptr<Material>& Mesh::material() const
{
	return m_material;
}

const AABB& Mesh::boundingBox() const
{
	return m_boundingBox;
}

Vec3 Mesh::vertex(int face, int v) const
{
	assert(face >= 0 && face < nbFaces());
	assert(v >= 0 && v < 3);

	const auto index = m_faces[face].vertices[v];

	assert(index >= 0 && index < nbVertices());
	
	return m_vertices[index].position;
}

Vec3 Mesh::normal(int face, int v) const
{
	assert(face >= 0 && face < nbFaces());
	assert(v >= 0 && v < 3);

	const auto index = m_faces[face].normals[v];

	if (index >= 0 && index < nbVertices())
	{
		// Normal is defined in the mesh
		return m_vertices[index].normal;
	}
	else
	{
		// We need to manually compute the normal
		const auto v0 = vertex(face, 0);
		const auto v1 = vertex(face, 1);
		const auto v2 = vertex(face, 2);

		return glm::normalize(glm::cross(v1 - v0, v2 - v0));
	}
}

Vec3 Mesh::normal(int face, double u, double v) const
{
	const auto n0 = normal(face, 0);
	const auto n1 = normal(face, 1);
	const auto n2 = normal(face, 2);

	const auto normalVector = (1.0 - u - v) * n0 + u * n1 + v * n2;

	return glm::normalize(normalVector);
}

void Mesh::updateBoundingBox()
{
	if (!m_vertices.empty())
	{
		Vec3 a = m_vertices.front().position;
		Vec3 b = m_vertices.front().position;

		for (const auto& v : m_vertices)
		{
			a = glm::min(a, v.position);
			b = glm::max(b, v.position);
		}

		m_boundingBox = AABB(a, b);
	}
}

int indexOfNumberLetter(const std::string& str, int offset)
{
	const auto length = static_cast<int>(str.length());

	for (int i = offset; i < length; ++i)
	{
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.') return i;
	}

	return length;
}

int lastIndexOfNumberLetter(const std::string& str)
{
	const auto length = static_cast<int>(str.length());

	for (int i = length - 1; i >= 0; --i)
	{
		if ((str[i] >= '0' && str[i] <= '9') || str[i] == '-' || str[i] == '.') return i;
	}

	return length;
}

std::vector<std::string> split(const std::string& s, char delim)
{
	std::vector<std::string> elems;

	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim))
	{
		elems.push_back(item);
	}

	return elems;
}

bool loadMesh(const std::filesystem::path& filename, Mesh& mesh)
{
	std::vector<MeshVertex> vertices;
	std::vector<MeshFace> faces;

	const auto success = loadMesh(filename, vertices, faces);

	mesh.setVertices(vertices);
	mesh.setFaces(faces);

	return success;
}

bool loadMesh(const std::filesystem::path& filename, std::vector<MeshVertex>& vertices, std::vector<MeshFace>& faces)
{
	std::ifstream file(filename);

	if (!file.is_open())
	{
		std::cerr << "Mesh::load() - Could not open file " << filename << std::endl;
		return false;
	}

	// Store vertex and normal data while reading
	std::vector<Vec3> raw_vertices;
	std::vector<Vec3> raw_normals;
	std::vector<int> v_elements;
	std::vector<int> n_elements;

	std::string line;
	while (getline(file, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			// Read position data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			auto values = split(line.substr(index1, index2 - index1 + 1), ' ');
			raw_vertices.emplace_back(stof(values[0]), stof(values[1]), stof(values[2]));
		}
		else if (line.substr(0, 3) == "vn ")
		{
			// Read normal data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			auto values = split(line.substr(index1, index2 - index1 + 1), ' ');
			raw_normals.emplace_back(stof(values[0]), stof(values[1]), stof(values[2]));
		}
		else if (line.substr(0, 2) == "f ")
		{
			// Read face data
			int index1 = indexOfNumberLetter(line, 2);
			int index2 = lastIndexOfNumberLetter(line);
			auto values = split(line.substr(index1, index2 - index1 + 1), ' ');
			for (int i = 0; i < static_cast<int>(values.size()) - 2; i++)
			{
				// Split up vertex indices
				auto v1 = split(values[0], '/'); // Triangle fan for ngons
				auto v2 = split(values[i + 1], '/');
				auto v3 = split(values[i + 2], '/');

				// Store position indices
				v_elements.push_back(std::stoul(v1[0]) - 1);
				v_elements.push_back(std::stoul(v2[0]) - 1);
				v_elements.push_back(std::stoul(v3[0]) - 1);

				// Check for normals
				if (v1.size() >= 3 && v1[2].length() > 0)
				{
					n_elements.push_back(std::stoul(v1[2]) - 1);
					n_elements.push_back(std::stoul(v2[2]) - 1);
					n_elements.push_back(std::stoul(v3[2]) - 1);
				}
			}
		}
	}

	file.close();

	// Resize vertices and faces vectors
	vertices.clear();
	vertices.reserve(raw_vertices.size());

	for (unsigned int i = 0; i < std::max(raw_vertices.size(), raw_normals.size()); i++)
	{
		Vec3 vertex;
		Vec3 normal;

		if (i < raw_vertices.size())
		{
			vertex = raw_vertices[i];
		}

		if (i < raw_normals.size())
		{
			normal = raw_normals[i];
		}

		vertices.emplace_back(vertex, normal);
	}

	faces.clear();
	faces.reserve(std::max(v_elements.size(), n_elements.size()) / 3);

	for (unsigned int i = 0; i < std::max(v_elements.size(), n_elements.size()); i += 3)
	{
		std::array<int, 3> v = { {0, 0, 0} };
		std::array<int, 3> n = { {-1, -1, -1} };

		if (i + 2 < v_elements.size())
		{
			v[0] = v_elements[i];
			v[1] = v_elements[i + 1];
			v[2] = v_elements[i + 2];
		}

		if (i + 2 < n_elements.size())
		{
			n[0] = n_elements[i];
			n[1] = n_elements[i + 1];
			n[2] = n_elements[i + 2];
		}

		faces.emplace_back(v, n);
	}

	return true;
}

bool rayMeshIntersection(
	const Mesh& mesh,
	const Ray& ray,
	double minT,
	HitRecord& hit)
{
	// First intersect ray with AABB to quickly discard non-intersecting rays
	if (!rayAABBIntersection(mesh.boundingBox(), ray))
	{
		return false;
	}

	const auto& orig = ray.origin();
	const auto& dir = ray.direction();
	
	bool intersectionFound = false;
	
	hit.ray = ray;
	hit.t = std::numeric_limits<double>::max();

	// Iterate over all triangles in the mesh
	for (int f = 0; f < mesh.nbFaces(); f++)
	{
		const auto& v0 = mesh.vertex(f, 0);
		const auto& v1 = mesh.vertex(f, 1);
		const auto& v2 = mesh.vertex(f, 2);

		// Distance between origin and hit along the ray
		double t;
		// Output barycentric coordinates of the intersection point
		Vec2 baryPosition;

		// Check if there is an intersection with this triangle
		if (glm::intersectRayTriangle(orig, dir, v0, v1, v2, baryPosition, t)
			&& t >= minT && t < hit.t)
		{
			hit.t = t;

			hit.point = ray.at(hit.t);			
			hit.normal = mesh.normal(f, baryPosition.x, baryPosition.y);
			hit.isFrontFace = glm::dot(ray.direction(), hit.normal) < 0;
			hit.material = mesh.material();

			intersectionFound = true;
		}
	}

	return intersectionFound;
}

bool rayMeshesIntersection(
	const std::vector<Mesh>& meshes,
	const Ray& ray,
	double minT,
	HitRecord& hit)
{	
	bool intersectionFound = false;

	hit.t = std::numeric_limits<double>::max();

	// Iterate over all triangles in the mesh
	for (unsigned int m = 0; m < meshes.size(); m++)
	{
		HitRecord meshHit;

		// Check if there is an intersection with this triangle
		if (rayMeshIntersection(meshes[m], ray, minT, meshHit) && meshHit.t < hit.t)
		{
			hit = meshHit;

			intersectionFound = true;
		}
	}

	return intersectionFound;
}
