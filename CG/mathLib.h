#pragma once
#include <glm/glm.hpp>
#include <cmath>

// Custom Vector3 Operations
class MathLib {
public:
    // Vector operations
    static glm::vec3 add(const glm::vec3& v1, const glm::vec3& v2);
    static glm::vec3 subtract(const glm::vec3& v1, const glm::vec3& v2);
    static glm::vec3 scale(const glm::vec3& v, float scalar);
    static glm::vec3 cross(const glm::vec3& v1, const glm::vec3& v2);
    static float dot(const glm::vec3& v1, const glm::vec3& v2);
    static glm::vec3 normalize(const glm::vec3& v);

    // Matrix operations
    static glm::mat4 multiply(const glm::mat4& m1, const glm::mat4& m2);
    static glm::vec3 transformPoint(const glm::mat4& matrix, const glm::vec3& point);

    // Transformation matrices
    static glm::mat4 identity();
    static glm::mat4 translation(float tx, float ty, float tz);
    static glm::mat4 scaling(float sx, float sy, float sz);
    static glm::mat4 rotationX(float angle);
    static glm::mat4 rotationY(float angle);
    static glm::mat4 rotationZ(float angle);
    static glm::mat4 perspective(float fov, float aspect, float nearPlane, float farPlane);
};
