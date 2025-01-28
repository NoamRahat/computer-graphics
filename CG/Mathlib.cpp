#include "MathLib.h"


#define M_PI 3.14159265358979323846


//Operations on 3 dimensional vectors
glm::vec3 MathLib::add(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::vec3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

glm::vec3 MathLib::subtract(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::vec3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

glm::vec3 MathLib::scale(const glm::vec3& v, float scalar) {
    return glm::vec3(v.x * scalar, v.y * scalar, v.z * scalar);
}

glm::vec3 MathLib::cross(const glm::vec3& v1, const glm::vec3& v2) {
    return glm::vec3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

float MathLib::dot(const glm::vec3& v1, const glm::vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

glm::vec3 MathLib::normalize(const glm::vec3& v) {
    float mag = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    return glm::vec3(v.x / mag, v.y / mag, v.z / mag);
}

//Operations on 4 dimensional matrixes

glm::mat4 MathLib::multiply(const glm::mat4& m1, const glm::mat4& m2) {
    glm::mat4 result(0.0f);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                result[i][j] += m1[i][k] * m2[k][j];
            }
        }
    }
    return result;
}

glm::vec3 MathLib::transformPoint(const glm::mat4& matrix, const glm::vec3& point) {
    glm::vec4 temp(point, 1.0f); // Convert to homogeneous coordinates
    glm::vec4 transformed = matrix * temp;
    return glm::vec3(transformed.x / transformed.w, transformed.y / transformed.w, transformed.z / transformed.w);
}

glm::mat4 MathLib::identity() {
    return glm::mat4(1.0f);
}

glm::mat4 MathLib::translation(float tx, float ty, float tz) {
    glm::mat4 mat = MathLib::identity();
    mat[3][0] = tx;
    mat[3][1] = ty;
    mat[3][2] = tz;
    return mat;
}

glm::mat4 MathLib::scaling(float sx, float sy, float sz) {
    glm::mat4 mat = MathLib::identity();
    mat[0][0] = sx;
    mat[1][1] = sy;
    mat[2][2] = sz;
    return mat;
}

glm::mat4 MathLib::rotationX(float angle) {
    glm::mat4 mat = MathLib::identity();
    float rad = angle * M_PI / 180.0f;
    mat[1][1] = std::cos(rad);
    mat[1][2] = -std::sin(rad);
    mat[2][1] = std::sin(rad);
    mat[2][2] = std::cos(rad);
    return mat;
}

glm::mat4 MathLib::rotationY(float angle) {
    glm::mat4 mat = MathLib::identity();
    float rad = angle * M_PI / 180.0f;
    mat[0][0] = std::cos(rad);
    mat[0][2] = std::sin(rad);
    mat[2][0] = -std::sin(rad);
    mat[2][2] = std::cos(rad);
    return mat;
}

glm::mat4 MathLib::rotationZ(float angle) {
    glm::mat4 mat = MathLib::identity();
    float rad = angle * M_PI / 180.0f;
    mat[0][0] = std::cos(rad);
    mat[0][1] = -std::sin(rad);
    mat[1][0] = std::sin(rad);
    mat[1][1] = std::cos(rad);
    return mat;
}

glm::mat4 MathLib::perspective(float fov, float aspect, float nearPlane, float farPlane) {
    glm::mat4 mat(0.0f);
    float tanHalfFov = std::tan(fov / 2.0f * M_PI / 180.0f);
    mat[0][0] = 1.0f / (aspect * tanHalfFov);
    mat[1][1] = 1.0f / tanHalfFov;
    mat[2][2] = -(farPlane + nearPlane) / (farPlane - nearPlane);
    mat[2][3] = -2.0f * farPlane * nearPlane / (farPlane - nearPlane);
    mat[3][2] = -1.0f;
    return mat;
}