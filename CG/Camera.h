#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();
    glm::mat4 getViewMatrix() const;
    glm::mat4 getProjectionMatrix(float aspectRatio) const;
    void setFrustumParameters(float n, float f, float r, float t);
    glm::mat4 getFrustumMatrix(float aspectRatio) const;

    void setPosition(const glm::vec3& position);
    void lookAt(const glm::vec3& target);
    void setPerspective(float fov, float nearPlane, float farPlane);
    void setUpVector(const glm::vec3& upVector);

    // Rotations
    void rotateY(float angleDegrees);  // Rotate around Y-axis
    void rotateX(float angleDegrees); // Rotate around X-axis
    void rotateZ(float angleDegrees);  // Rotate around Z-axis
    void orbitAroundTarget(float angleDegrees, const glm::vec3& axis); // Orbit around target

    //getters
    glm::vec3 getPosition() const;
    glm::vec3 getTarget() const;
    glm::vec3 getUpVector() const;
    float getFov() const;
    float getNearPlane() const;
    float getFarPlane() const;

    //setters
    //void setPosition(const glm::vec3& position);
    void setTarget(const glm::vec3& target);
    //void setUpVector(const glm::vec3& upVector);
    void setFov(float fov);
    void setNearPlane(float nearPlane);
    void setFarPlane(float farPlane);


private:
    glm::vec3 position , forward, up, target;
    float fov ,  nearPlane, farPlane, right, top;


    void validatePerspectiveParameters() const;
    void updateOrientation();
};