#include "Camera.h"
#include <stdexcept>
#include "MathLib.h"
 //custom matrix calculations library


// ====== Constructor ====== //
Camera::Camera() :
    position(0.0f, 0.0f, 5.0f),  // Default camera position
    target(0.0f, 0.0f, 0.0f),    // Default target at origin
    up(0.0f, 1.0f, 0.0f),        // Default up vector (Y-axis)
    fov(45.0f),                  // Default field of view (FoV)
    nearPlane(0.1f),             // Default near clipping plane
    farPlane(1000.0f) {          // Default far clipping plane
}


// ====== Setters ====== //


// Set camera position
void Camera::setPosition(const glm::vec3& pos) {
    position = pos;
}

// Update frustum parameters (used in projection matrix)
void Camera::setFrustumParameters(float n, float f, float r, float t) {
    nearPlane = n;
    farPlane = f;
    right = r;
    top = t;

    // Validate the frustum parameters
    if (nearPlane <= 0 || farPlane <= nearPlane) {
        throw std::invalid_argument("Invalid near or far plane.");
    }
    if (right <= 0 || top <= 0) {
        throw std::invalid_argument("Invalid right or top values.");
    }
}

// Set perspective projection parameters
void Camera::setPerspective(float fieldOfView, float nearPlane, float farPlane) {
    this->fov = fieldOfView;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;
    validatePerspectiveParameters();
}

// Set the up vector (re-normalized)
void Camera::setUpVector(const glm::vec3& upVector) {
    up = MathLib::normalize(upVector); // Use MathLib for normalization
}

// Point the camera at a specific target
void Camera::lookAt(const glm::vec3& tgt) {
    target = tgt; // Update the target
    forward = MathLib::normalize(target - position); // Recalculate forward vector
    glm::vec3 right = MathLib::normalize(MathLib::cross(forward, up)); // Calculate right vector
    setUpVector(MathLib::cross(right, forward)); // Update up vector
}



// ====== Getters ====== //


// Compute the view matrix
glm::mat4 Camera::getViewMatrix() const {
    glm::vec3 forward = MathLib::normalize(target - position);
    glm::vec3 right = MathLib::normalize(MathLib::cross(forward, up));
    glm::vec3 newUp = MathLib::cross(right, forward);

    // Create a custom look-at matrix
    glm::mat4 rotation = MathLib::identity();
    rotation[0][0] = right.x; rotation[1][0] = right.y; rotation[2][0] = right.z;
    rotation[0][1] = newUp.x; rotation[1][1] = newUp.y; rotation[2][1] = newUp.z;
    rotation[0][2] = -forward.x; rotation[1][2] = -forward.y; rotation[2][2] = -forward.z;

    glm::mat4 translation = MathLib::translation(-position.x, -position.y, -position.z);
    return MathLib::multiply(rotation, translation); // Combine rotation and translation
}


// Compute the frustum matrix (manual frustum setup)
glm::mat4 Camera::getFrustumMatrix(float aspectRatio) const {
    return MathLib::perspective(fov, aspectRatio, nearPlane, farPlane);
}

// Compute the perspective projection matrix (using FoV and aspect ratio)
glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const {
    validatePerspectiveParameters();
    return MathLib::perspective(fov, aspectRatio, nearPlane, farPlane);
}

// Get camera position
glm::vec3 Camera::getPosition() const {
    return position;
}

// Get camera target
glm::vec3 Camera::getTarget() const {
    return target;
}

// Get up vector
glm::vec3 Camera::getUpVector() const {
    return up;
}

// Get field of view
float Camera::getFov() const {
    return fov;
}

// Get near plane
float Camera::getNearPlane() const {
    return nearPlane;
}

// Get far plane
float Camera::getFarPlane() const {
    return farPlane;
}

// ====== Validation ====== //

// Validate the perspective parameters
void Camera::validatePerspectiveParameters() const {
    if (fov <= 0 || fov >= 180) {
        throw std::invalid_argument("Field of view must be between 0 and 180 degrees.");
    }
    if (nearPlane <= 0 || farPlane <= nearPlane) {
        throw std::invalid_argument("Near plane must be greater than 0 and less than far plane.");
    }
}