// BBox.h
#pragma once
#include "Renderer.h"
#include <glm/vec3.hpp>
#include <cfloat>
#include <vector>

class Renderer; // Forward declaration

class BBox {
public:
    BBox();
    void compute(const std::vector<glm::vec3>& vertices);
    void draw(Renderer& renderer, unsigned int color) const;

private:
    glm::vec3 minCorner;
    glm::vec3 maxCorner;
};
