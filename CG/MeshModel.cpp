#include "MeshModel.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cfloat>

MeshModel::MeshModel() {}

bool MeshModel::loadFromOBJFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filePath << std::endl
            << ". Please check the file path and permissions." << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream ss(line);
        std::string prefix;
        ss >> prefix;

        // Handel vertices
        if (prefix == "v") {
            glm::vec3 vertex;
            ss >> vertex.x >> vertex.y >> vertex.z;
            vertices.push_back(vertex);
        }
        // Handle faces (support vertices for simplicity)
        else if (prefix == "f") {
            unsigned int v1, v2, v3;
            char slash;
            ss >> v1 >> slash >> slash >> v2 >> slash >> slash >> v3;
            indices.push_back(v1 - 1); // Convert 1-based indecies to 0-based
            indices.push_back(v2 - 1);
            indices.push_back(v3 - 1);
        }
    }
    file.close();

    normalizeModel(); // Transform to canonical coordinates
    computeNormals(); // Compute vertex normals
    updateBoundingBox(); // Recompute bounding box after loading and normalizing
    return true;
}

void MeshModel::normalizeModel() {
    glm::vec3 minPoint(FLT_MAX), maxPoint(-FLT_MAX);
    for (const auto& vertex : vertices) {
        minPoint = glm::min(minPoint, vertex);
        maxPoint = glm::max(maxPoint, vertex);
    }

    glm::vec3 center = (minPoint + maxPoint) / 2.0f;
    float maxLength = glm::length(maxPoint - minPoint);

    for (auto& vertex : vertices) {
        vertex = (vertex - center) / maxLength;
    }
}

void MeshModel::computeNormals() {
    normals.resize(vertices.size(), glm::vec3(0.0f));
    for (size_t i = 0; i < indices.size(); i += 3) {
        glm::vec3 v0 = vertices[indices[i]];
        glm::vec3 v1 = vertices[indices[i + 1]];
        glm::vec3 v2 = vertices[indices[i + 2]];

        glm::vec3 normal = glm::normalize(glm::cross(v1 - v0, v2 - v0));

        normals[indices[i]] += normal;
        normals[indices[i + 1]] += normal;
        normals[indices[i + 2]] += normal;
    }
    for (auto& normal : normals) {
        normal = glm::normalize(normal);
    }
}

void MeshModel::updateBoundingBox() {
    boundingBox.compute(vertices); // Recompute bounding box based on current vertices
}

const BBox& MeshModel::getBoundingBox() const {
    return boundingBox;
}

const std::vector<glm::vec3>& MeshModel::getVertices() const { return vertices; }
const std::vector<glm::vec3>& MeshModel::getNormals() const { return normals; }
const std::vector<unsigned int>& MeshModel::getIndices() const { return indices; }
