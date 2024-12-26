#include <iostream>

#include <AntTweakBar/include/AntTweakBar.h>
#include <Glew/include/gl/glew.h>
#include <freeglut/include/GL/freeglut.h>

#include <vector>
#include <Windows.h>
#include <assert.h>
#include <iostream>

#include "Utils.h"
#include "Renderer.h"
#include "Obj Parser/wavefront_obj.h"


#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>


LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

double g_Scale = 1.0;
//double g_quaternion[4] = {0.0, 0.0, 0.0, 1.0};

//points coordinates
int g_P1x = 20, g_P1y = 20; //P1
int g_P2x = 500, g_P2y = 400; //P2

//obj data type
Wavefront_obj objScene;

void TW_CALL loadOBJModel(void* clientData);
void initScene();
void initGraphics(int argc, char *argv[]);
void drawScene();
void Display();
void Reshape(int width, int height);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void PassiveMouseMotion(int x, int y);
void Keyboard(unsigned char k, int x, int y);
void Special(int k, int x, int y);
void Terminate(void);


int main(int argc, char *argv[])
{
	// Initialize openGL, glut, glew
	initGraphics(argc, argv);
	// Initialize AntTweakBar
	TwInit(TW_OPENGL, NULL);
	//initialize the timer frequency
	QueryPerformanceFrequency(&Frequency); 
	// Set GLUT callbacks
	glutDisplayFunc(Display);
	glutReshapeFunc(Reshape);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(PassiveMouseMotion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	
	//send 'glutGetModifers' function pointer to AntTweakBar.
	//required because the GLUT key event functions do not report key modifiers states.
	//TwGLUTModifiersFunc(glutGetModifiers);


	atexit(Terminate);  //called after glutMainLoop ends


	// Create a tweak bar
	TwBar* bar = TwNewBar("TweakBar");

	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar with GLUT and OpenGL.' "); // Message added to the help bar.
	TwDefine(" TweakBar size='200 400' color='96 216 224' "); // change default tweak bar size and color
	//the x,y coordinates
	TwAddVarRW(bar, "P1.x", TW_TYPE_UINT16, &g_P1x, " min=0 max=1500 step=1 keyIncr=x keyDecr=X help='Point 1 x coordinate' ");
	TwAddVarRW(bar, "P1.y", TW_TYPE_UINT16, &g_P1y, " min=0 max=750 step=1 keyIncr=y keyDecr=Y help='Point 1 y coordinate' ");
	TwAddVarRW(bar, "P2.x", TW_TYPE_UINT16, &g_P2x, " min=0 max=1500 step=1 keyIncr=a keyDecr=A help='Point 2 x coordinate' ");
	TwAddVarRW(bar, "P2.y", TW_TYPE_UINT16, &g_P2y, " min=0 max=750 step=1 keyIncr=b keyDecr=B help='Point 2 y coordinate' ");
	TwAddVarRO(bar, "time (us)", TW_TYPE_UINT32, &ElapsedMicroseconds.LowPart, "help='shows the drawing time in micro seconds'");
	
	//add 'g_Scale' to 'bar': this is a modifiable (RW) variable of type TW_TYPE_DOUBLE. Its key shortcuts are [s] and [S].
	TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &g_Scale, " min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Scale the object (1=original size).' ");

	//add 'g_quaternion' to 'bar': this is a variable of type TW_TYPE_QUAT4D which defines the object's orientation using quaternions
	//TwAddVarRW(bar, "ObjRotation", TW_TYPE_QUAT4D, &g_quaternion, " label='Object rotation' opened=true help='Change the object orientation.' ");

	TwAddButton(bar, "open", loadOBJModel, NULL, " label='Open OBJ File...' ");



	// Call the GLUT main loop
	glutMainLoop();

	return 0;
}


void TW_CALL loadOBJModel(void *data)
{
	std::wstring str = getOpenFileName();

	bool result = objScene.load_file(str);

	if(result)
	{
		std::cout << "The obj file was loaded successfully" << std::endl;
	}
	else
	{
		std::cerr << "Failed to load obj file" << std::endl;
	}

	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;

}


//do not change this function unless you really know what you are doing!
void initGraphics(int argc, char *argv[])
{
	// Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(960, 640);
	glutCreateWindow("Computer Graphics Skeleton using AntTweakBar and freeGlut");
	glutCreateMenu(NULL);

	// Initialize OpenGL
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_NORMALIZE);
	glDisable(GL_LIGHTING);
	glColor3f(1.0, 0.0, 0.0);

	// Initialize GLEW
	GLenum err = glewInit();
	if(err != GLEW_OK)
	{
		assert(0);
		return;
	}
}


//this is just an example of usage for the Renderer::drawPixels function.
//remove these lines and use your own code for drawing the scene.
//it is also a good idea to move the drawScene() function to another file/class
void drawScene()
{


	glm::dmat4x4 model(1.0, 0.0, 0.0, 0.0,   0.0, 1.0, 0.0, 0.0,    0.0, 0.0, 1.0, 0.0,    0.0, 0.0, 0.0, 1.0);
	glm::vec4 vec(1.0, 1.0, 1.0, 1.0);
	glm::vec4 res = model*vec;
	res = vec*model;


	static int k = 1;

	Renderer renderer;
	std::vector<Pixel> pixels;
	pixels.reserve(100000); //consider changing this if more pixels are colored in the next assignments

	bool blue = true;

	for(int i = 0; i < 400; i++)
	{
		if(i%k == 0)
		{
			blue = !blue;
		}

		for(int j = 0; j < 100; j++)
		{
			Pixel pixel;
			pixel.x = i;
			pixel.y = j;

			if(blue)
			{
				pixel.color = 0xffff0000;
			}
			else //red
			{
				pixel.color = 0xff0000ff;
			}
			pixels.push_back(pixel);
		}
	}
	renderer.drawPixels(pixels);
	k++;
}


//this will make sure that integer coordinates are mapped exactly to corresponding pixels on screen
void glUseScreenCoordinates(int width, int height)
{
	// Set OpenGL viewport and camera
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


// Callback function called by GLUT to render screen
void Display()
{
//  	static int counter = 0;
//  	std::cout << "C: " << counter << std::endl;
//  	counter++;

    glClearColor(0, 0, 0, 1); //background color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//time measuring - don't delete
	QueryPerformanceCounter(&StartingTime);

 	drawScene();

	//time measuring - don't delete
	QueryPerformanceCounter(&EndingTime);
	ElapsedMicroseconds.QuadPart = EndingTime.QuadPart - StartingTime.QuadPart;
	ElapsedMicroseconds.QuadPart *= 1000000;
	ElapsedMicroseconds.QuadPart /= Frequency.QuadPart;

	// Draw tweak bars
	TwDraw();

	//swap back and front frame buffers
	glutSwapBuffers();
}


// Callback function called by GLUT when window size changes
void Reshape(int width, int height)
{
	glUseScreenCoordinates(width, height);

	//////////////////////////////////////
	///////add your reshape code here/////////////



	//////////////////////////////////////

    // Send the new window size to AntTweakBar
    TwWindowSize(width, height);
	glutPostRedisplay();
}



void MouseButton(int button, int state, int x, int y)
{
	TwEventMouseButtonGLUT(button, state, x, y);
	glutPostRedisplay();
}

void MouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
	glutPostRedisplay();
}

void PassiveMouseMotion(int x, int y)
{
	TwEventMouseMotionGLUT(x, y);
}


void Keyboard(unsigned char k, int x, int y)
{
	TwEventKeyboardGLUT(k, x, y);
	glutPostRedisplay();
}


void Special(int k, int x, int y)
{
	TwEventSpecialGLUT(k, x, y);
	glutPostRedisplay();
}


// Function called at exit
void Terminate(void)
{ 
    TwTerminate();
}


