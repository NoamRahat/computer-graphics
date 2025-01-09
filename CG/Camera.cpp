#include"camera.h"

Camera::Camera() {
    updateViewMatrix();
    updateProjectionMatrix(16,9);
}
Camera::~Camera()
{
}

void Camera::updateViewMatrix() {
    glm::vec3 f = glm::normalize(position-target );         // Forward vector
    glm::vec3 r = glm::normalize(glm::cross({ 0, 1, 0 }, f));  // Right vector
    glm::vec3 u = glm::cross(f, r);                          // Up vector

    viewMatrix = glm::mat4(
        r.x, u.x, f.x, 0.0f, // Column 1
        r.y, u.y, f.y, 0.0f, // Column 2
        r.z, u.z, f.z, 0.0f, // Column 3
        -glm::dot(r, position), -glm::dot(u, position), -glm::dot(f, position), 1.0f // Column 4
    );
}



// Compute projection matrix
void Camera:: updateProjectionMatrix(int h,int w) {
    //not sure what aspect ratio neen to be so this is the other option
    //aspectRatio = float(w) / float(h);
    if (CameraType) {
        double tanHalfFovY = std::tan(glm::radians(fov));
        //projectionMatrix = glm::mat4x4(1/(aspectRatio* tan(glm::radians(fov)/2)), 0.0f, 0.0f, 0.0f,//first column
        //    0.0f, 1 / tan(glm::radians(fov) /2), 0.0f, 0.0f,//second column
        //    0.0f, 0.0f, -float((farPlane + nearPlane)) / float((farPlane - nearPlane)),-1.0f ,//third column
        //    0.0f, 0.0f, (-2 * float(farPlane * nearPlane)) / float((farPlane - nearPlane)), 0.0f);
        projectionMatrix[0][0] = 1.0 / (aspectRatio * tanHalfFovY); // Scale X
        projectionMatrix[1][1] = 1.0 / tanHalfFovY;           // Scale Y
        projectionMatrix[2][2] = -(farPlane + n) / (farPlane - n);          // Z normalization
        projectionMatrix[2][3] = -1.0;                        // Perspective division
        projectionMatrix[3][2] = -(2.0 * farPlane * n) / (farPlane - n);    // Depth offset


    }
    else {
        projectionMatrix = glm::mat4x4(n / r, 0.0f, 0.0f, 0.0f,//first column
            0.0f, n / t, 0.0f, 0.0f,//second column
            0.0f, 0.0f, -float((farPlane + n)) / float((farPlane - n)), (-2 * float(farPlane * n)) / float((farPlane - n)),//third column
            0.0f, 0.0f, -1.0f, 0.0f);
    }
   

}

glm::dmat4x4 perspectiveFoV(float fov, float aspect, float n, float f) {
    // Convert field of view from degrees to radians
   

    // Construct the perspective matrix
    glm::dmat4x4 perspective(0.0);


    return perspective;
}

