#pragma once
#include "BBox.h"
#include <string>


class MeshModel {
private:
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;
    BBox boundingBox; // Bounding box member

    void normalizeModel();

public:
    MeshModel();
    bool loadFromOBJFile(const std::string& filePath);
    void computeNormals();

    void updateBoundingBox(); // Recompute the bounding box
    const BBox& getBoundingBox() const; // Getter for bounding box

    const std::vector<glm::vec3>& getVertices() const;
    const std::vector<glm::vec3>& getNormals() const;
    const std::vector<unsigned int>& getIndices() const;
};
