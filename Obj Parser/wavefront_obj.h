/////////////////////////////////////////////////////////////////////////////////////////////////////////
// This simple obj parser assumes  that all polygons in the mesh are triangles and will fail otherwise //
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include <iostream>
#include <fstream>
#include <assert.h>
#include <vector>
#include <string>
#include <glm/vec3.hpp>
#include "../CG/BBox.h"
// Forward declaration of BBox
class BBox;

struct Wavefront_obj
{
public:
    BBox boundingBox;
    const BBox& getBoundingBox() const { return boundingBox; }

    struct Face //each face is a triangle
    {
        Face() { v[0] = 0; v[1] = 0; v[2] = 0; t[0] = 0; t[1] = 0; t[2] = 0; n[0] = 0; n[1] = 0; n[2] = 0; }

        int v[3];
        int t[3];
        int n[3];
    };

    struct Vector {
        // Constructors
        Vector() : e{ 0.0, 0.0, 0.0 } {}
        Vector(double x, double y, double z) : e{ x, y, z } {}
        Vector(double val) : e{ val, val, val } {}

        double e[3];

        inline double& operator[](int index) { return e[index]; }
        inline const double& operator[](int index) const { return e[index]; }

        // Overload + operator for vector addition
        Vector operator+(const Vector& other) const {
            return Vector(e[0] + other.e[0], e[1] + other.e[1], e[2] + other.e[2]);
        }

        // Overload - operator for vector subtraction
        Vector operator-(const Vector& other) const {
            return { e[0] - other.e[0], e[1] - other.e[1], e[2] - other.e[2] };
        }

        // Overload / operator for scalar division
        Vector operator/(double scalar) const {
            return { e[0] / scalar, e[1] / scalar, e[2] / scalar };
        }

        // Convert to glm::vec3
        glm::vec3 toGLMVec3() const {
            return glm::vec3(static_cast<float>(e[0]), static_cast<float>(e[1]), static_cast<float>(e[2]));
        }
    };

    std::vector<Vector> m_points;
    std::vector<Vector> m_normals;
    std::vector<Vector> m_textureCoordinates;
    std::vector<Face> m_faces;

    bool load_file(std::wstring filename);
    void Wavefront_obj::canonicalTransform();

    void clear() {
        m_points.clear();
        m_normals.clear();
        m_textureCoordinates.clear();
        m_faces.clear();
        boundingBox = BBox(); // Reset bounding box
    }

};
