#include "Wavefront_obj.h"

std::vector<glm::vec3> convertToGLMVec3(const std::vector<Wavefront_obj::Vector>& points) {
	std::vector<glm::vec3> glmPoints;
	glmPoints.reserve(points.size());
	for (const auto& point : points) {
		glmPoints.emplace_back(point[0], point[1], point[2]);
	}
	return glmPoints;
}

bool Wavefront_obj::load_file(std::wstring filename)
{
	char line[8192];

	std::ifstream objFile;
	objFile.open(filename.c_str());

	if (!objFile.is_open())
	{
		std::cerr << "Could not open file " << filename.c_str() << std::endl;
		return false;
	}

	while (objFile.getline(line, 8192))
	{

		//vertex position
		if (!strncmp(line, "v ", 2)) {
			Vector v;
			sscanf_s(line + 2, "%lf %lf %lf", &v[0], &v[1], &v[2]);
			m_points.push_back(v);
		}

		//texture coordinates
		if (!strncmp(line, "vt ", 2)) {
			Vector v;
			sscanf_s(line + 2, "%lf %lf %lf", &v[0], &v[1], &v[2]);
			m_textureCoordinates.push_back(v);
		}

		//normal vector
		if (!strncmp(line, "vn ", 2)) {
			Vector n;
			sscanf_s(line + 2, "%lf %lf %lf", &n[0], &n[1], &n[2]);
			m_normals.push_back(n);
		}

		//face
		if (!strncmp(line, "f ", 2)) {
			Face f;

			//each vertex of this triangle has position, texture coordinates and normal
			if (sscanf_s(line + 2, "%d/%d/%d %d/%d/%d %d/%d/%d", &f.v[0], &f.t[0], &f.n[0], &f.v[1], &f.t[1], &f.n[1], &f.v[2], &f.t[2], &f.n[2]) == 9)
			{

			}
			//each vertex of this triangle only has position (no normals and no texture coordinates)
			else if (sscanf_s(line + 2, "%d %d %d", &f.v[0], &f.v[1], &f.v[2]) == 3)
			{

			}
			//each vertex of this triangle has position and normal (but not texture coordinates)
			else if (sscanf_s(line + 2, "%d//%d %d//%d %d//%d", &f.v[0], &f.n[0], &f.v[1], &f.n[1], &f.v[2], &f.n[2]) == 6)
			{

			}
			else
			{
				//unsupported mode
				assert(0);
			}

			//subtract 1 since obj file format uses 1-based indexing and C++ uses 0-based indexing.
			//since all indices are initialized to 0, a missing index value will be converted from 0 to -1.
			f.v[0]--;
			f.v[1]--;
			f.v[2]--;

			f.t[0]--;
			f.t[1]--;
			f.t[2]--;

			f.n[0]--;
			f.n[1]--;
			f.n[2]--;

			m_faces.push_back(f);
		}

	}
	objFile.close();

	//validity check
	int numFaces = m_faces.size();
	int numVertices = m_points.size();
	int numNormals = m_normals.size();
	int numTextureCoordinates = m_textureCoordinates.size();

	assert(numFaces >= 1); //a valid model has at least one face
	assert(numVertices >= 3); //a valid model has at least three vertices
	if (numFaces == 0 || numVertices < 3) {
		std::cerr << "Error: Invalid OBJ file. Must have at least one face and three vertices." << std::endl;
		return false;
	}


	for (int i = 0; i < numFaces; i++)
	{
		Face& f = m_faces[i];

		assert(f.v[0] >= 0 && f.v[0] < numVertices);
		assert(f.v[1] >= 0 && f.v[1] < numVertices);
		assert(f.v[2] >= 0 && f.v[2] < numVertices);

		if (numNormals > 0)
		{
			assert(f.n[0] >= 0 && f.n[0] < numNormals);
			assert(f.n[1] >= 0 && f.n[1] < numNormals);
			assert(f.n[2] >= 0 && f.n[2] < numNormals);
		}
		else //no normals provided
		{
			assert(f.n[0] == -1);
			assert(f.n[1] == -1);
			assert(f.n[2] == -1);
		}

		if (numTextureCoordinates > 0)
		{
			assert(f.t[0] >= 0 && f.t[0] < numTextureCoordinates);
			assert(f.t[1] >= 0 && f.t[1] < numTextureCoordinates);
			assert(f.t[2] >= 0 && f.t[2] < numTextureCoordinates);
		}
		else //no texture coordinates provided
		{
			assert(f.t[0] == -1);
			assert(f.t[1] == -1);
			assert(f.t[2] == -1);
		}
	}

	std::cout << "Before canonical Transform: " << std::endl;

	canonicalTransform(); // Normalize the model

	std::cout << "Before computing vertex normals: " << std::endl;

	computeVertexNormals();

	if (m_points.size() >= 3) {
		std::vector<glm::vec3> glmPoints = convertToGLMVec3(m_points);
		std::cout << "boundingBox computing: " << std::endl;
		boundingBox.compute(glmPoints); // Compute the bounding box using vertices
	}


	return true;
}

void Wavefront_obj::canonicalTransform() {
	Vector minCorner(FLT_MAX);
	Vector maxCorner(-FLT_MAX);

	// Calculate min and max corners
	for (const auto& v : m_points) {
		for (int i = 0; i < 3; ++i) {
			minCorner[i] = std::min(minCorner[i], v[i]);
			maxCorner[i] = std::max(maxCorner[i], v[i]);
		}
	}

	// Compute center and scale
	Vector center = (minCorner + maxCorner) / 2.0;
	double scale = std::max(maxCorner[0] - minCorner[0],
		std::max(maxCorner[1] - minCorner[1], maxCorner[2] - minCorner[2]));

	// Normalize points
	for (auto& v : m_points) {
		for (int i = 0; i < 3; ++i) {
			v[i] = (v[i] - center[i]) / scale * 2.0; // Normalize to [-1, 1]
		}
	}


}

void Wavefront_obj::computeVertexNormals() {
	// Resize normals to match the number of vertices
	m_normals.resize(m_points.size(), Vector(0.0, 0.0, 0.0));

	// Iterate over all faces
	for (const auto& face : m_faces) {
		// Get the vertices of the triangle
		Vector v0 = m_points[face.v[0]];
		Vector v1 = m_points[face.v[1]];
		Vector v2 = m_points[face.v[2]];

		// Compute the normal for the triangle (not normalized)
		Vector edge1 = v1 - v0;
		Vector edge2 = v2 - v0;
		Vector normal = Vector(
			edge1[1] * edge2[2] - edge1[2] * edge2[1],
			edge1[2] * edge2[0] - edge1[0] * edge2[2],
			edge1[0] * edge2[1] - edge1[1] * edge2[0]
		);

		// Accumulate the triangle normal for each vertex
		m_normals[face.v[0]] = m_normals[face.v[0]] + normal;
		m_normals[face.v[1]] = m_normals[face.v[1]] + normal;
		m_normals[face.v[2]] = m_normals[face.v[2]] + normal;
	}

	// Normalize all vertex normals
	for (auto& normal : m_normals) {
		double length = std::sqrt(normal[0] * normal[0] +
			normal[1] * normal[1] +
			normal[2] * normal[2]);
		if (length > 0.0) {
			normal[0] /= length;
			normal[1] /= length;
			normal[2] /= length;
		}
	}

	std::cout << "Computed weighted vertex normals." << std::endl;
}