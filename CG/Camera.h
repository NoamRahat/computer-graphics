#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
class Camera 
{
public:    
    ~Camera();
    Camera();
    // Projection parameters
    float farPlane = 1000.0f;
    float fov = 45.0f; // Default FoV in degrees
    float aspectRatio = 1; // Default aspect ratio
    float t= 1;
    float r=1;
    float n=1;
    bool CameraType = 1;
    //// Camera position and orientation
    glm::vec3 position = glm::vec3(0.0f, 0.0f,6.0f); // Default position
    glm::vec3 target = glm::vec3(0.0f, 0.0f, 0.0f); // Looking at the origin
    
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    void updateProjectionMatrix(int H,int W);
    void updateViewMatrix();

protected:
};