#include "Camera.h"
#include <stdexcept>

Camera::Camera() :
    position(0.0f, 0.0f, 5.0f),
    target(0.0f, 0.0f, 0.0f),
    up(0.0f, 1.0f, 0.0f),
    fov(45.0f),
    nearPlane(0.1f),
    farPlane(1000.0f) {
}

void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}

void Camera::lookAt(const glm::vec3& tgt) {
    target = tgt;
}

void Camera::setPerspective(float fieldOfView, float nearPlane, float farPlane) {
    this->fov = fieldOfView;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    validatePerspectiveParameters();
}

void Camera::setUpVector(const glm::vec3& upVector) {
    up = glm::normalize(upVector);
}

void Camera::validatePerspectiveParameters() const {
    if (fov <= 0 || fov >= 180) {
        throw std::invalid_argument("Field of view must be between 0 and 180 degrees.");
    }
    if (nearPlane <= 0 || farPlane <= nearPlane) {
        throw std::invalid_argument("Near plane must be greater than 0 and less than far plane.");
    }
}


//getters
glm::mat4 Camera::getViewMatrix() const {
    return glm::lookAt(position, target, up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    validatePerspectiveParameters();
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

glm::vec3 Camera::getPosition() const {
    return position;
}

glm::vec3 Camera::getTarget() const {
    return target;
}

glm::vec3 Camera::getUpVector() const {
    return up;
}

float Camera::getFov() const {
    return fov;
}

float Camera::getNearPlane() const {
    return nearPlane;
}

float Camera::getFarPlane() const {
    return farPlane;
}