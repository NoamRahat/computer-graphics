#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>
// <vector>
#include <Windows.h>
#include <assert.h>
#include <iostream>
#include <cassert>

#include "Utils.h"
#include "Renderer.h"
#include "GlobalSettings.h"
#include "../Obj Parser/wavefront_obj.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>



// Timer variables
LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

// Global variables for shapes and objects
Wavefront_obj objScene;
Renderer renderer;

// Forward declarations
void setupTweakBar();
void loadOBJModel();
void initGraphics(int argc, char* argv[]);
void drawScene();
void display();
void reshape(int width, int height);
void mouseButton(int button, int state, int x, int y);
void mouseMotion(int x, int y);
void passiveMouseMotion(int x, int y);
void keyboard(unsigned char key, int x, int y);
void specialKeys(int key, int x, int y);
void cleanUp();
void glUseScreenCoordinates(int width, int height);

int main(int argc, char* argv[])
{
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
    glutPassiveMotionFunc(passiveMouseMotion);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeys);

    // Cleanup on exit
    atexit(cleanUp);

    // Start GLUT main loop
    glutMainLoop();

    return 0;
}

void setupTweakBar() {
    TwBar* bar = TwNewBar("Settings");

    // Add shape mode selection
    TwEnumVal shapeModes[] = {
        {LINE, "Line"}, {SQUARE, "Square"}, {CIRCLE, "Circle"},
        {TRIANGLE, "Triangle"}, {STAR, "Star"}, {PENTAGON, "Pentagon"}
    };
    TwType shapeType = TwDefineEnum("ShapeMode", shapeModes, 6);
    TwAddVarRW(bar, "Shape Mode", shapeType, &GlobalSettings::currentShapeMode, "help='Select shape mode.'");

    // Add controls for line coordinates
    TwAddVarRW(bar, "Line Start X", TW_TYPE_INT32, &GlobalSettings::lineStartX, "min=0 max=1500 step=1 group='Line'");
    TwAddVarRW(bar, "Line Start Y", TW_TYPE_INT32, &GlobalSettings::lineStartY, "min=0 max=750 step=1 group='Line'");
    TwAddVarRW(bar, "Line End X", TW_TYPE_INT32, &GlobalSettings::lineEndX, "min=0 max=1500 step=1 group='Line'");
    TwAddVarRW(bar, "Line End Y", TW_TYPE_INT32, &GlobalSettings::lineEndY, "min=0 max=750 step=1 group='Line'");

    // Add controls for shape size
    TwAddVarRW(bar, "Shape Size", TW_TYPE_INT32, &GlobalSettings::shapeSize, "min=10 max=500 step=10");

    // Add controls for circle
    TwAddVarRW(bar, "Circle Center X", TW_TYPE_INT32, &GlobalSettings::circleCenterX, "min=0 max=1500 step=1 group='Circle'");
    TwAddVarRW(bar, "Circle Center Y", TW_TYPE_INT32, &GlobalSettings::circleCenterY, "min=0 max=750 step=1 group='Circle'");
    TwAddVarRW(bar, "Circle Radius", TW_TYPE_INT32, &GlobalSettings::circleRadius, "min=10 max=500 step=1 group='Circle'");

    // Add color picker
    TwAddVarRW(bar, "Shape Color", TW_TYPE_COLOR32, &GlobalSettings::lineColor, "help='Select shape or line color.'");
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

    std::cout << "Frame time (microseconds): " << ElapsedMicroseconds.QuadPart << std::endl;

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

void mouseButton(int button, int state, int x, int y) {
    if (!TwEventMouseButtonGLUT(button, state, x, y)) {}
    glutPostRedisplay();
}

void mouseMotion(int x, int y) {
    if (!TwEventMouseMotionGLUT(x, y)) {}
    glutPostRedisplay();
}

void passiveMouseMotion(int x, int y) {
    if (!TwEventMouseMotionGLUT(x, y)) {}
}

void keyboard(unsigned char key, int x, int y) {
    if (!TwEventKeyboardGLUT(key, x, y)) {}
    glutPostRedisplay();
}

void specialKeys(int key, int x, int y) {
    if (!TwEventSpecialGLUT(key, x, y)) {}
    glutPostRedisplay();
}

void cleanUp() {
    TwTerminate();
}
