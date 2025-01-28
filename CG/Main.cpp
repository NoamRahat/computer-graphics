#include <iostream>
#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>
#include <Windows.h>
#include <cassert>
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include "MathLib.h"

#include "Utils.h"
#include "Renderer.h"
#include "GlobalSettings.h"
#include "../Obj Parser/wavefront_obj.h"
#include "MeshModel.h"
#include "Camera.h"

using namespace std;
using namespace GlobalSettings;
using namespace glm;
#define EX1
// Timer variables
LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

// Global variables for shapes and objects
bool showBoundingBox = false;
bool showVertexNormals = false;
bool showWorldAxis = false;
Wavefront_obj objScene;
Renderer renderer;
Camera camera; // Global camera object
//MeshModel meshModel;
std::string loadedFileName = "No file loaded";
int vertexCount = 0;
int faceCount = 0;


// Forward declarations
void setupTweakBar();
void TW_CALL loadOBJModel(void* clientData);
void TW_CALL focusOnObjectCallback(void* userData); 
void focusOnObject(Camera& camera);
void initGraphics(int argc, char* argv[]);
void drawScene();
void display();
void reshape(int width, int height);
void mouseButton(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void mouseWheel(int button, int dir, int x, int y);
void passiveMouseMotion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void cleanUp();
void glUseScreenCoordinates(int width, int height);
glm::vec3 toScreenSpace(const glm::vec4& vertex, int screenWidth, int screenHeight);

int main(int argc, char* argv[])
{
    // Initialize camera
    camera.setPosition(glm::vec3(0.0f, 0.0f, 10.0f)); // Move camera back
    camera.lookAt(glm::vec3(0.0f, 0.0f, 0.0f));        // Look at the object

    // Initialize openGL, glut, glew
    initGraphics(argc, argv);

    // Initialize AntTweakBar
    TwInit(TW_OPENGL, NULL);

    // Initialize timer frequency
    QueryPerformanceFrequency(&Frequency);

    // Setup tweak bar
    setupTweakBar();

    // GLUT callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutMouseWheelFunc(mouseWheel);
    glutPassiveMotionFunc(passiveMouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    // Cleanup on exit
    atexit(cleanUp);

    // Start GLUT main loop
    glutMainLoop();

    return 0;
}


///////////////////////////////////
// Setting GUI Bar
//////////////////////////////////

void setupTweakBar() {
    TwBar* bar = TwNewBar("Settings");

    // Add shape mode selection
    TwEnumVal shapeModes[] = {
        {LINE, "Line"}, {SQUARE, "Square"}, {CIRCLE, "Circle"},
        {TRIANGLE, "Triangle"}, {STAR, "Star"}, {PENTAGON, "Pentagon"}
    };
    TwType shapeType = TwDefineEnum("ShapeMode", shapeModes, 6);
    TwAddVarRW(bar, "Shape Mode", shapeType, &currentShapeMode, "help='Select shape mode.'");

    // Add controls for line coordinates
    TwAddVarRW(bar, "Line Start X", TW_TYPE_INT32, &lineStartX, "min=0 max=1500 step=1 group='Line'");
    TwAddVarRW(bar, "Line Start Y", TW_TYPE_INT32, &lineStartY, "min=0 max=750 step=1 group='Line'");
    TwAddVarRW(bar, "Line End X", TW_TYPE_INT32, &lineEndX, "min=0 max=1500 step=1 group='Line'");
    TwAddVarRW(bar, "Line End Y", TW_TYPE_INT32, &lineEndY, "min=0 max=750 step=1 group='Line'");

    // Add controls for shape size
    TwAddVarRW(bar, "Shape Size", TW_TYPE_INT32, &shapeSize, "min=10 max=500 step=10");

    // Add controls for circle
    TwAddVarRW(bar, "Circle Center X", TW_TYPE_INT32, &circleCenterX, "min=0 max=1500 step=1 group='Circle'");
    TwAddVarRW(bar, "Circle Center Y", TW_TYPE_INT32, &circleCenterY, "min=0 max=750 step=1 group='Circle'");
    TwAddVarRW(bar, "Circle Radius", TW_TYPE_INT32, &circleRadius, "min=10 max=500 step=1 group='Circle'");

    // Add color picker
    TwAddVarRW(bar, "Shape Color", TW_TYPE_COLOR32, &lineColor, "help='Select shape or line color.'");
    TwAddVarRO(bar, "time (us)", TW_TYPE_UINT32, &ElapsedMicroseconds.LowPart, "help='shows the drawing time in micro seconds'");

    TwAddVarRO(bar, "File Name", TW_TYPE_STDSTRING, &loadedFileName, "label='Loaded File'");
    TwAddVarRO(bar, "Vertex Count", TW_TYPE_INT32, &vertexCount, "label='Vertices'");
    TwAddVarRO(bar, "Face Count", TW_TYPE_INT32, &faceCount, "label='Faces'");
    TwAddButton(bar, "open", loadOBJModel, NULL, " label='Open OBJ File...' ");
    TwAddButton(bar, "Switch Model", loadOBJModel, NULL, "label='Load Another Model'");
    TwAddButton(bar, "Focus on Object", focusOnObjectCallback, &camera, " label='Focus on Object' ");
    TwAddVarRW(bar, "Show Bounding Box", TW_TYPE_BOOLCPP, &showBoundingBox, "label='Show Bounding Box'");
    TwAddVarRW(bar, "Show Vertex Normals", TW_TYPE_BOOLCPP, &showVertexNormals, "label='Show Vertex Normals'");
    TwAddVarRW(bar, "Show World Axis", TW_TYPE_BOOLCPP, &showWorldAxis, "label='Show World Axis'");
    TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &g_Scale, " min=0.01 max=2.5 step=0.01 keyIncr=h keyDecr=H help='Scale the object.' ");
    TwAddButton(bar, "Exit", [](void*) { exit(0); }, NULL, "label='Exit'");

}

///////////////////////////////////
// Functionality for GUI Bar
//////////////////////////////////

void TW_CALL loadOBJModel(void* data) {
    std::wstring str = getOpenFileName();

    if (str.empty()) {
        std::cerr << "No file selected." << std::endl;
        return;
    }

    // Clear the previous object data
    objScene.clear();

    // Load the new OBJ file
    bool result = objScene.load_file(str);

    if (result) {
        loadedFileName = std::string(str.begin(), str.end());
        vertexCount = objScene.m_points.size();
        faceCount = objScene.m_faces.size();
        std::cout << "OBJ loaded: " << loadedFileName << std::endl;

        if (vertexCount < 3 || faceCount < 1) {
            std::cerr << "Invalid OBJ file. Minimum 3 vertices and 1 face required." << std::endl;
            return;
        }
   
        glutPostRedisplay();
    }
    else {
        std::cerr << "Failed to load OBJ file." << std::endl;
        loadedFileName = "Failed to load";
        vertexCount = 0;
        faceCount = 0;
    }

    std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
    std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;

    for (const auto& vertex : objScene.m_points) {
        std::cout << "Vertex: (" << vertex[0] << ", " << vertex[1] << ", " << vertex[2] << ")" << std::endl;
    }

}

void initGraphics(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Interactive Renderer");

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glDisable(GL_NORMALIZE);
    glDisable(GL_LIGHTING);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        assert(0);
        return;
    }
}

void drawScene() {
    glm::mat4 viewMatrix = camera.getViewMatrix();
    glm::mat4 projectionMatrix = camera.getProjectionMatrix(800.0f / 600.0f); // Update aspect ratio dynamically

    glm::mat4 vpMatrix = projectionMatrix * viewMatrix;
    if (!objScene.m_faces.empty()) {
        
		// Adding axis to the scene
        if (showWorldAxis) {
            std::cout << "showing World Axis:" << std::endl;
            renderer.drawCoordinateSystem(vpMatrix, 50.0f); // Scale = 5.0 for visibility
        }

        // Adding BoundingBox to the scene
        if (showBoundingBox) {
            std::cout << "showing Bounding Box :" << std::endl;
            objScene.boundingBox.draw(renderer, 0xFF00FF, camera, 800.0f / 600.0f); // Magenta color
        }

		// Drawing the object to the scene using the vertex and face data
        for (const auto& face : objScene.m_faces) {
            
            // Transform vertices to clip space
            glm::vec4 v1 = vpMatrix * glm::vec4(objScene.m_points[face.v[0]].toGLMVec3(), 1.0f);
            glm::vec4 v2 = vpMatrix * glm::vec4(objScene.m_points[face.v[1]].toGLMVec3(), 1.0f);
            glm::vec4 v3 = vpMatrix * glm::vec4(objScene.m_points[face.v[2]].toGLMVec3(), 1.0f);

            // Transform vertices to screen space
            glm::vec3 v1Screen = toScreenSpace(v1, 800, 600);
            glm::vec3 v2Screen = toScreenSpace(v2, 800, 600);
            glm::vec3 v3Screen = toScreenSpace(v3, 800, 600);


            // Draw the edges of the triangle (wireframe)
            renderer.drawLine(static_cast<int>(v1Screen.x), static_cast<int>(v1Screen.y),
                static_cast<int>(v2Screen.x), static_cast<int>(v2Screen.y), 0xFFFFFF);

            renderer.drawLine(static_cast<int>(v2Screen.x), static_cast<int>(v2Screen.y),
                static_cast<int>(v3Screen.x), static_cast<int>(v3Screen.y), 0xFFFFFF);

            renderer.drawLine(static_cast<int>(v3Screen.x), static_cast<int>(v3Screen.y),
                static_cast<int>(v1Screen.x), static_cast<int>(v1Screen.y), 0xFFFFFF);

            // Adding Vertexes normals to the scene
            if (showVertexNormals){
                // Calculate the face normal
                glm::vec3 p1 = objScene.m_points[face.v[0]].toGLMVec3();
                glm::vec3 p2 = objScene.m_points[face.v[1]].toGLMVec3();
                glm::vec3 p3 = objScene.m_points[face.v[2]].toGLMVec3();

                glm::vec3 edge1 = p2 - p1;
                glm::vec3 edge2 = p3 - p1;
                glm::vec3 normal = MathLib::normalize(MathLib::cross(edge1, edge2)); // Face normal

                // Calculate the face center
                glm::vec3 faceCenter = (p1 + p2 + p3) / 3.0f;

                // Transform the face center and normal to clip space
                glm::vec4 faceCenterClip = vpMatrix * glm::vec4(faceCenter, 1.0f);
                glm::vec4 normalEndClip = vpMatrix * glm::vec4(faceCenter + normal * 0.5f, 1.0f); // Scale normal for visualization

                // Convert to screen space
                glm::vec3 faceCenterScreen = toScreenSpace(faceCenterClip, 800, 600);
                glm::vec3 normalEndScreen = toScreenSpace(normalEndClip, 800, 600);

                // Draw the normal as a line
                renderer.drawLine(static_cast<int>(faceCenterScreen.x), static_cast<int>(faceCenterScreen.y),
                    static_cast<int>(normalEndScreen.x), static_cast<int>(normalEndScreen.y), 0xFF0000); // Red color for normals

            }
        }

    }
    else {
#ifdef EX1
        switch (GlobalSettings::currentShapeMode) {
        case LINE:
            renderer.drawLine(GlobalSettings::lineStartX, GlobalSettings::lineStartY,
                GlobalSettings::lineEndX, GlobalSettings::lineEndY,
                GlobalSettings::lineColor);
            break;
        case SQUARE:
            renderer.drawSquare(300, 300, GlobalSettings::shapeSize, GlobalSettings::lineColor);
            break;
        case CIRCLE:
            renderer.drawCircle(GlobalSettings::circleCenterX, GlobalSettings::circleCenterY,
                GlobalSettings::circleRadius, GlobalSettings::lineColor);
            break;
        case TRIANGLE:
            renderer.drawTriangle(400, 400, GlobalSettings::shapeSize, GlobalSettings::lineColor);
            break;
        case STAR:
            renderer.drawStar(400, 400, GlobalSettings::shapeSize, GlobalSettings::lineColor);
            break;
        case PENTAGON:
            renderer.drawPentagon(400, 400, GlobalSettings::shapeSize, GlobalSettings::lineColor);
            break;
        }
#endif // EX1
    }
}

void display() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QueryPerformanceCounter(&StartingTime);

    drawScene();

    QueryPerformanceCounter(&EndingTime);
    ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
    ElapsedMicroseconds.QuadPart *= 1000000;
    ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

    TwDraw();
    glutSwapBuffers();
}

void reshape(int width, int height) {
    glUseScreenCoordinates(width, height);
    TwWindowSize(width, height);
    glutPostRedisplay();
}

void glUseScreenCoordinates(int width, int height) {
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, width, 0, height, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

float lastMouseX = 0.0f, lastMouseY = 0.0f;
float rotationX = 0.0f, rotationY = 0.0f;

///////////////////////////////////
// View functions
//////////////////////////////////


void adjustFoV(float zoomStep, int zoomDirection) {
    // Decrease FoV for zoom in, increase for zoom out
    float newFoV = camera.getFov() + (zoomDirection > 0 ? -zoomStep : zoomStep);

    // Clamp FoV to reasonable values
    float minFoV = 10.0f; // Very zoomed-in
    float maxFoV = 90.0f; // Very zoomed-out
    if (newFoV < minFoV) newFoV = minFoV;
    if (newFoV > maxFoV) newFoV = maxFoV;

    // Update the camera's FoV
    camera.setPerspective(newFoV, camera.getNearPlane(), camera.getFarPlane());
    glutPostRedisplay(); // Redraw the scene
}


void focusOnObject(Camera& camera) {
    glm::vec3 objectCenter = glm::vec3(0.0f, 0.0f, 0.0f); // Object at origin
    float focusDistance = 10.0f; // Default distance to ensure the object is visible

    // Set the camera position and look at the object
    glm::vec3 cameraPosition = objectCenter + glm::vec3(0.0f, 0.0f, focusDistance); // Move the camera back
    camera.setPosition(cameraPosition);
    camera.lookAt(objectCenter);
}

void TW_CALL focusOnObjectCallback(void* userData) {
    Camera* cam = static_cast<Camera*>(userData);
    focusOnObject(*cam); // Focus on the object at the origin
    glutPostRedisplay(); // Redraw the scene
}

///////////////////////////////////
// keyboard and mouse functions
//////////////////////////////////

void mouseWheel(int button, int dir, int x, int y) {
    float zoomStep = 1.0f; 
    adjustFoV(zoomStep, dir); // dir > 0 for zoom in, dir < 0 for zoom out
}

void mouseButton(int button, int state, int x, int y) {
    if (!TwEventMouseButtonGLUT(button, state, x, y)) {}
    if (state == GLUT_DOWN) {
        lastMouseX = x;
        lastMouseY = y;
    }
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (!TwEventMouseMotionGLUT(x, y)) {}

    float deltaX = x - lastMouseX;
    float deltaY = y - lastMouseY;


    // Update camera rotation 
    rotationX += deltaY * 0.1f; // Vertical mouse movement
    rotationY += deltaX * 0.1f; // Horizontal mouse movement 

    // Clamp vertical rotation to avoid flipping
    if (rotationX > 89.0f) rotationX = 89.0f;
    if (rotationX < -89.0f) rotationX = -89.0f;

    // Calculate the new view direction for the camera
    glm::vec3 direction;
    direction.x = cos(glm::radians(rotationY)) * cos(glm::radians(rotationX));
    direction.y = sin(glm::radians(rotationX));
    direction.z = sin(glm::radians(rotationY)) * cos(glm::radians(rotationX));

    // Update the camera to look in the new direction
    camera.lookAt(camera.getPosition() + MathLib::normalize(direction));

    lastMouseX = x;
    lastMouseY = y;

    glutPostRedisplay(); // Refresh the scene
}

void passiveMouseMotion(int x, int y) {
    if (!TwEventMouseMotionGLUT(x, y)) {}
}


void keyboard(unsigned char key, int x, int y) {
    if (!TwEventKeyboardGLUT(key, x, y)) {}

    // Calculate local axes based on the camera's current orientation
    glm::vec3 forward = MathLib::normalize(camera.getTarget() - camera.getPosition());
    glm::vec3 right = MathLib::normalize(MathLib::cross(forward, camera.getUpVector()));
    glm::vec3 up = camera.getUpVector();

	// Camera movement in the "world"
    switch (key) {
        // Scaling: scales the model uniformly in all directions.
        case 'w': camera.setPosition(camera.getPosition() + glm::vec3(0.0f, 0.1f, 0.0f)); break;
        case 's': camera.setPosition(camera.getPosition() + glm::vec3(0.0f, -0.1f, 0.0f)); break;
        
        // Translation (x, y, z): Moves the model in space
        case 'a': camera.setPosition(camera.getPosition() + glm::vec3(-0.1f, 0.0f, 0.0f)); break;
        case 'd': camera.setPosition(camera.getPosition() + glm::vec3(0.1f, 0.0f, 0.0f)); break;
    }

    
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (!TwEventSpecialGLUT(key, x, y)) {}
    glutPostRedisplay();
}

void cleanUp() {
    TwTerminate();
}

