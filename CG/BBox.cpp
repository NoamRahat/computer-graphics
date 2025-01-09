#include "BBox.h"
#include <cfloat>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <freeglut/include/GL/freeglut.h>

BBox::BBox() : minCorner(glm::vec3(FLT_MAX)), maxCorner(glm::vec3(-FLT_MAX)) {}

void BBox::compute(const std::vector<glm::vec3>& vertices) {
    minCorner = glm::vec3(FLT_MAX);
    maxCorner = glm::vec3(-FLT_MAX);

    for (const auto& vertex : vertices) {
        minCorner = glm::min(minCorner, vertex);
        maxCorner = glm::max(maxCorner, vertex);
    }
}

void BBox::draw(Renderer& renderer, unsigned int color) const {
    glm::vec3 corners[8] = {
        minCorner,
        {minCorner.x, minCorner.y, maxCorner.z},
        {minCorner.x, maxCorner.y, minCorner.z},
        {minCorner.x, maxCorner.y, maxCorner.z},
        {maxCorner.x, minCorner.y, minCorner.z},
        {maxCorner.x, minCorner.y, maxCorner.z},
        {maxCorner.x, maxCorner.y, minCorner.z},
        maxCorner
    };

    int edges[12][2] = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0}, // Bottom face
        {4, 5}, {5, 7}, {7, 6}, {6, 4}, // Top face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
    };

    for (const auto& edge : edges) {
        renderer.drawLine(corners[edge[0]], corners[edge[1]], color);
    }
}
