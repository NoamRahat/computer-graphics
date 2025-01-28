#include "BBox.h"
#include "Utils.h"

#include <cfloat>
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <freeglut/include/GL/freeglut.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

//constructor - 
BBox::BBox() : minCorner(glm::vec3(FLT_MAX)), maxCorner(glm::vec3(-FLT_MAX)) {}


// calculate the min/max corners of the BBox
void BBox::compute(const std::vector<glm::vec3>& vertices) {
    minCorner = glm::vec3(FLT_MAX);
    maxCorner = glm::vec3(-FLT_MAX);

    for (const auto& vertex : vertices) {
        minCorner = glm::min(minCorner, vertex);
        maxCorner = glm::max(maxCorner, vertex);
    }

    // Add a small depth if the bounding box is flat on the z-axis
    if (std::abs(maxCorner.z - minCorner.z) < 1e-5f) { // Check for flatness
        maxCorner.z += 0.1f; // Add a small depth
    }

    std::cout << "BBox computed: MinCorner = " << glm::to_string(minCorner)
        << ", MaxCorner = " << glm::to_string(maxCorner) << std::endl;
}

// draws the bounding box as a wireframe by connecting its corners with edges.
void BBox::draw(Renderer& renderer, unsigned int color , const Camera& camera, float aspectRatio) const {

    // get the combined view projection matrix from the renderer.
    // this matrix transforms world-space coordinates into clip space.
   
    glm::mat4 vpMatrix = renderer.getViewProjectionMatrix(camera, aspectRatio);

    // define the 8 corners of the bounding box in world space.

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


    for (int i = 0; i < 8; ++i) {
        std::cout << "Corner[" << i << "]: " << glm::to_string(corners[i]) << std::endl;
    }

    // define the 12 edges of the bounding box, each edge connects two corners
    
    int edges[12][2] = {
        {0, 1}, {1, 3}, {3, 2}, {2, 0}, // Bottom face
        {4, 5}, {5, 7}, {7, 6}, {6, 4}, // Top face
        {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Vertical edges
    };


    std::cout << "Original corner positions:" << std::endl;
    for (int i = 0; i < 8; ++i) {
        std::cout << "Corner[" << i << "] = " << glm::to_string(corners[i]) << std::endl;
    }



    // iterate through each edge and draw a line between its two corners.
    for (const auto& edge : edges) {

        
        glm::vec4 start = vpMatrix * glm::vec4(corners[edge[0]], 1.0f);
        glm::vec4 end = vpMatrix * glm::vec4(corners[edge[1]], 1.0f);

        glm::vec3 screenStart = toScreenSpace(start, 800, 600);
        glm::vec3 screenEnd = toScreenSpace(end, 800, 600);

        renderer.drawLine(screenStart, screenEnd, color);

        renderer.drawLine(corners[0], corners[2], 0x00FF00);


        // Convert the NDC coordinates into screen space coordinates.
       // This ensures the coordinates are mapped to the screen resolution.
       //renderer.drawLine(toScreenSpace(start, 800, 600), toScreenSpace(end, 800, 600), color);
    }
}
