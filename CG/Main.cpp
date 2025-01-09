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
#include "camera.h"
#include "Object.h"

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>


LARGE_INTEGER StartingTime, EndingTime, ElapsedMicroseconds;
LARGE_INTEGER Frequency;

double g_Scale = 1.0;
//double g_quaternion[4] = {0.0, 0.0, 0.0, 1.0};

//points coordinates
int g_P1x = 750, g_P1y = 375; //P1
int g_P2x = 350, g_P2y = 300; //P2
int g_P3x = 600, g_P3y = 200; //P3
unsigned int colorRGB = 0xffffffff; //defult color -yellow
float IL1[] = { 1.0,1.0, 1.0 }; //defult color -yellow
float IL2[] = { 1.0,1.0, 1.0 }; //defult color -yellow
float ILA[] = { 1.0,1.0, 1.0 }; //defult color -yellow
unsigned int colorObj = 0xffffff; //defult color -yellow
Renderer renderer;
int Radius = 150; //rad
int mode = 0; //drawing mode - 1) line 2) triangle 3)squere 4)circle 5)star
int L1mode = 0; //light mode - 0)directional 1)point
int L2mode = 0; //light mode - 0)directional 1)point
int shadingmode = 0; //light mode - 0)directional 1)point
int BufferMode = 0; //light mode - 0)directional 1)point
float x = 0.0f, y = 0.0f, z = 0.0f, dx = 0.0f, dy = 0.0f, dz = 0.0f, sx = 1.0f, sy = 1.0f, sz = 1.0f, scale = 1.0f, cx = 0.0f, cy = 0.0f, cz = 0.0f;
float xL1 = 0.0f, yL1 = 0.0f, zL1 = 0.0f, xL2 = 0.0f, yL2 = 0.0f, zL2 = 0.0f,Nsize;
bool result = 0;
bool ShowWorldAxis = 0;
bool ShowObjectAxis = 0;
bool showCenter = 0;
bool L2Toggle = 0;
bool BBprintflag = 0;
bool printVertixNormalsflag = 0;
bool doubleSide = 0;
//obj data type
Wavefront_obj objScene;
Object obj;
void TW_CALL loadOBJModel(void* clientData);
void initScene();
void initGraphics(int argc, char* argv[]);
void drawScene(int x1, int y1, int x2);
void Display();
void Reshape(int width, int height);
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);
void PassiveMouseMotion(int x, int y);
void Keyboard(unsigned char k, int x, int y);
void Special(int k, int x, int y);
void Terminate(void);
void TW_CALL wroldTranslation(void* clientData);
void TW_CALL objectTranslation(void* clientData);
void TW_CALL WorldRotationX(void* clientData);
void TW_CALL WorldRotationY(void* clientData);
void TW_CALL WorldRotationZ(void* clientData);
void TW_CALL ObjectRotationX(void* clientData);
void TW_CALL ObjectRotationY(void* clientData);
void TW_CALL ObjectRotationZ(void* clientData);
void TW_CALL WorldScale(void* clientData);
void TW_CALL ObjectScale(void* clientData);
void TW_CALL BBToggle(void* clientData);
void TW_CALL printNormalToggle(void* clientData);
void TW_CALL lookAt(void* clientData);
void TW_CALL cameraTranslation(void* clientData);
void TW_CALL ObjectAxisToggle(void* clientData);
void TW_CALL ObjectCenterToggle(void* clientData);
void TW_CALL WorldAxisToggle(void* clientData);
void TW_CALL ChangeCameraSetting(void* clientData);
void TW_CALL UpdatecameraParam(void* clientData);
int main(int argc, char* argv[])



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
	enum LightType {
		Directional = 0,
		Point = 1
	};

	enum shading {
		wireframe = 0,
		flat = 1,
		goro = 2,
		phong = 3
	};
	enum bin {
		off = 0,
		on = 1
	};
	enum Buffer {
		Mode0 = 0,
		Mode1 = 1,
		Mode2 = 2,
	};
	// Array for enum value descriptions
	TwEnumVal lightTypeEV[] = {
   {Directional, "Directional"}, // Map to Directional
	{Point, "Point"},
	};
	TwEnumVal shadingTypeEV[] = {
	{wireframe, "wireframe"},
	{flat, "flat"}, // Map to Directional
	{goro, "goro"},
	{phong, "phong"},

	};
	TwEnumVal binTypeEV[] = {
   {off, "off"},
   {on, "on"}, // Map to Directional

	};
	TwEnumVal BufferTypeEV[] = {
	{Mode0, "f(z)"},
	{Mode1, "pix-invert-f(z)"},
	{Mode2, "vertix-invert-f(z)"},
	// Map to Directional

	};
	// Create a tweak bar
	TwBar* bar = TwNewBar("TweakBar");

	// General settings
	TwAddVarRO(bar, "time (us)", TW_TYPE_UINT32, &ElapsedMicroseconds.LowPart, "help='Shows the drawing time in microseconds' group='General'");
	TwAddButton(bar, "open", loadOBJModel, NULL, "label='Open OBJ File...' group='General'");

	// Light settings
	TwType lightTypeEnum = TwDefineEnum("LightType", lightTypeEV, 2);
	TwType lightTypeEnum2 = TwDefineEnum("LightType2", lightTypeEV, 2);
	TwType binTypeEnum = TwDefineEnum("on/off", binTypeEV, 2);
	TwType shadingTypeEnum = TwDefineEnum("shadingType", shadingTypeEV, 4);
	TwType BufferTypeEnum = TwDefineEnum("Buffer", BufferTypeEV, 3);

	TwAddVarRW(bar, "Shading Type", shadingTypeEnum, &shadingmode, "label='Shading Type' help='Select the shading technique (flat, goro, phong).' group='Shading'");

	// Object settings
	TwAddVarRW(bar, "L2Toggle", TW_TYPE_BOOL8, &L2Toggle, "help='Toggle L2' group='Object'");
	TwAddVarRW(bar, "colorObj", TW_TYPE_COLOR32, &colorObj, "help='Object color' group='Object'");
	TwAddVarRW(bar, "shininess", TW_TYPE_INT16, &obj.n, "min=1 max=200 step=1 label='Shininess' group='Object'");
	TwAddVarRW(bar, "specular", TW_TYPE_FLOAT, &obj.Ks, "min=0 max=1 step=0.1 label='Specular' group='Object'");
	TwAddVarRW(bar, "diffusion", TW_TYPE_FLOAT, &obj.Kd, "min=0 max=1 step=0.1 label='Diffusion' group='Object'");
	TwAddVarRW(bar, "ambiant", TW_TYPE_FLOAT, &obj.Ka, "min=0 max=1 step=0.1 label='Ambient' group='Object'");
	TwAddVarRW(bar, "doubleSide", TW_TYPE_BOOL8, &doubleSide, "help='Double side' group='Object'");
	TwAddVarRW(bar, "zBufferMode", BufferTypeEnum, &BufferMode, "help='Z Buffer Mode' group='Object'");

	// Ambient light settings
	TwAddVarRW(bar, "colorRGBLA", TW_TYPE_COLOR3F, &ILA, "help='Ambient light color' group='Ambient Light'");

	// Light 1 settings
	TwAddVarRW(bar, "LightTypeL1", lightTypeEnum, &L1mode, "label='Light Type L1' help='Select whether the light source L1 is Directional or Point.' group='Light1'");
	TwAddVarRW(bar, "lightdir1", TW_TYPE_DIR3D, &obj.light.light1, "help='Light direction' group='Light1'");
	TwAddVarRW(bar, "colorRGBL1", TW_TYPE_COLOR3F, &IL1, "help='Light color' group='Light1'");
	TwAddVarRW(bar, "X-L1", TW_TYPE_FLOAT, &xL1, "label='X Value' step=0.1 group='Light1'");
	TwAddVarRW(bar, "Y-L1", TW_TYPE_FLOAT, &yL1, "label='Y Value' step=0.1 group='Light1'");
	TwAddVarRW(bar, "Z-L1", TW_TYPE_FLOAT, &zL1, "label='Z Value' step=0.1 group='Light1'");

	// Light 2 settings
	TwAddVarRW(bar, "LightTypeL2", lightTypeEnum2, &L2mode, "label='Light Type L2' help='Select whether the light source L2 is Directional or Point.' group='Light2'");
	TwAddVarRW(bar, "lightdir2", TW_TYPE_DIR3D, &obj.light.light2, "help='Light direction' group='Light2'");
	TwAddVarRW(bar, "colorRGBL2", TW_TYPE_COLOR3F, &IL2, "help='Light color' group='Light2'");
	TwAddVarRW(bar, "X-L2", TW_TYPE_FLOAT, &xL2, "label='X Value' step=0.1 group='Light2'");
	TwAddVarRW(bar, "Y-L2", TW_TYPE_FLOAT, &yL2, "label='Y Value' step=0.1 group='Light2'");
	TwAddVarRW(bar, "Z-L2", TW_TYPE_FLOAT, &zL2, "label='Z Value' step=0.1 group='Light2'");

	// Shape settings
	TwAddVarRW(bar, "Mode", TW_TYPE_UINT16, &mode, "min=0 max=4 step=1 help='Control shape' group='Shape'");
	TwAddVarRW(bar, "colorRGB", TW_TYPE_COLOR32, &colorRGB, "help='Shape color' group='Shape'");

	// Camera settings
	TwAddVarRO(bar, "Object X", TW_TYPE_FLOAT, &obj.origen.x, "label='Object X' group='Location'");
	TwAddVarRO(bar, "Object Y", TW_TYPE_FLOAT, &obj.origen.y, "label='Object Y' group='Location'");
	TwAddVarRO(bar, "Object Z", TW_TYPE_FLOAT, &obj.origen.z, "label='Object Z' group='Location'");
	TwAddVarRW(bar, "BB-toggle", TW_TYPE_BOOL8, &BBprintflag, "help='Bounding box toggle' group='Camera'");
	TwAddVarRW(bar, "printVertixNormals", TW_TYPE_BOOL8, &printVertixNormalsflag, "help='Print vertex normals' group='Camera'");
	TwAddVarRW(bar, "objectAxis", TW_TYPE_BOOL8, &ShowObjectAxis, "help='Show object axis' group='Camera'");
	TwAddVarRW(bar, "worldAxis", TW_TYPE_BOOL8, &ShowWorldAxis, "help='Show world axis' group='Camera'");
	TwAddVarRW(bar, "showCenter", TW_TYPE_BOOL8, &showCenter, "help='Show center' group='Camera'");
	TwAddButton(bar, "lookAt", lookAt, NULL, "label='Look at' group='Camera'");
	TwAddVarRW(bar, "Nsize", TW_TYPE_FLOAT, &obj.Nsize, "min=0 label='Normal size' group='Camera'");

	TwAddVarRO(bar, "Camera X", TW_TYPE_FLOAT, &obj.camera.position.x, "label='Camera X' group='Location'");
	TwAddVarRO(bar, "Camera Y", TW_TYPE_FLOAT, &obj.camera.position.y, "label='Camera Y' group='Location'");
	TwAddVarRO(bar, "Camera Z", TW_TYPE_FLOAT, &obj.camera.position.z, "label='Camera Z' group='Location'");

	TwAddVarRW(bar, "cX", TW_TYPE_FLOAT, &cx, "label='cX Value' step=0.1 group='Camera'");
	TwAddVarRW(bar, "cY", TW_TYPE_FLOAT, &cy, "label='cY Value' step=0.1 group='Camera'");
	TwAddVarRW(bar, "cZ", TW_TYPE_FLOAT, &cz, "label='cZ Value' step=0.1 group='Camera'");
	TwAddButton(bar, "ccamera", cameraTranslation, NULL, "label='Change Camera Position' group='Camera'");
	TwAddVarRW(bar, "near", TW_TYPE_FLOAT, &obj.camera.n, "label='Near' step=0.1 group='CameraSetting'");
	TwAddVarRW(bar, "far", TW_TYPE_FLOAT, &obj.camera.farPlane, "label='Far Value' step=0.1 group='CameraSetting'");
	TwAddVarRW(bar, "right", TW_TYPE_FLOAT, &obj.camera.r, "label='Right Value' step=0.1 group='CameraSetting'");
	TwAddVarRW(bar, "top", TW_TYPE_FLOAT, &obj.camera.t, "label='Top Value' step=0.1 group='CameraSetting'");
	TwAddVarRW(bar, "fov", TW_TYPE_FLOAT, &obj.camera.fov, "label='FOV Value' step=0.1 group='CameraSetting'");
	TwAddButton(bar, "", ChangeCameraSetting, NULL, "label='Change Camera Setting' group='CameraSetting'");

	// Translation settings
	TwAddVarRW(bar, "X", TW_TYPE_FLOAT, &x, "label='X Value' step=0.1 group='Translate'");
	TwAddVarRW(bar, "Y", TW_TYPE_FLOAT, &y, "label='Y Value' step=0.1 group='Translate'");
	TwAddVarRW(bar, "Z", TW_TYPE_FLOAT, &z, "label='Z Value' step=0.1 group='Translate'");
	TwAddButton(bar, "world1", wroldTranslation, NULL, "label='World Translate' group='Translate'");
	TwAddButton(bar, "obj1", objectTranslation, NULL, "label='Object Translate' group='Translate'");

	// Rotation settings
	TwAddVarRW(bar, "dX", TW_TYPE_FLOAT, &dx, "label='X Rotation' step=0.1 group='Rotate'");
	TwAddVarRW(bar, "dY", TW_TYPE_FLOAT, &dy, "label='Y Rotation' step=0.1 group='Rotate'");
	TwAddVarRW(bar, "dZ", TW_TYPE_FLOAT, &dz, "label='Z Rotation' step=0.1 group='Rotate'");
	TwAddButton(bar, "WorldRotX", WorldRotationX, NULL, "label='World X' group='Rotate'");
	TwAddButton(bar, "WorldRotY", WorldRotationY, NULL, "label='World Y' group='Rotate'");
	TwAddButton(bar, "WorldRotZ", WorldRotationZ, NULL, "label='World Z' group='Rotate'");
	TwAddButton(bar, "ObjectRotX", ObjectRotationX, NULL, "label='Object X' group='Rotate'");
	TwAddButton(bar, "ObjectRotY", ObjectRotationY, NULL, "label='Object Y' group='Rotate'");
	TwAddButton(bar, "ObjectRotZ", ObjectRotationZ, NULL, "label='Object Z' group='Rotate'");

	// Scale settings
	TwAddVarRW(bar, "AllScale", TW_TYPE_FLOAT, &scale, "label='All Scale' step=0.1 group='Scale'");
	TwAddButton(bar, "WorldScale", WorldScale, NULL, "label='World Scale' group='Scale'");
	TwAddButton(bar, "objectScale", ObjectScale, NULL, "label='Object Scale' group='Scale'");

	// Point settings
	TwAddVarRW(bar, "Radius", TW_TYPE_UINT16, &Radius, "min=0 max=500 step=1 keyIncr=x keyDecr=X help='Radius size for mode 3' group='Point1'");
	TwAddVarRW(bar, "P1.x", TW_TYPE_UINT16, &g_P1x, "min=0 max=1500 step=1 keyIncr=x keyDecr=X help='Point 1 x coordinate' group='Point1' label='x'");
	TwAddVarRW(bar, "P1.y", TW_TYPE_UINT16, &g_P1y, "min=0 max=750 step=1 keyIncr=y keyDecr=Y help='Point 1 y coordinate' group='Point1' label='y'");
	TwAddVarRW(bar, "P2.x", TW_TYPE_UINT16, &g_P2x, "min=0 max=1500 step=1 keyIncr=a keyDecr=A help='Point 2 x coordinate' group='Point2' label='x'");
	TwAddVarRW(bar, "P2.y", TW_TYPE_UINT16, &g_P2y, "min=0 max=750 step=1 keyIncr=b keyDecr=B help='Point 2 y coordinate' group='Point2' label='y'");
	TwAddVarRW(bar, "P3.x", TW_TYPE_UINT16, &g_P3x, "min=0 max=1500 step=1 keyIncr=a keyDecr=A help='Point 3 x coordinate' group='Point3' label='x'");
	TwAddVarRW(bar, "P3.y", TW_TYPE_UINT16, &g_P3y, "min=0 max=750 step=1 keyIncr=b keyDecr=B help='Point 3 y coordinate' group='Point3' label='y'");

	// Scale settings
	TwAddVarRW(bar, "Scale", TW_TYPE_DOUBLE, &g_Scale, "min=0.01 max=2.5 step=0.01 keyIncr=s keyDecr=S help='Scale the object (1=original size).' group='Scale'");

	// Define tweak bar properties
	TwDefine("TweakBar size='300 800' color='96 216 224' text=large");
	TwDefine("TweakBar/General opened=true");
	TwDefine("TweakBar/Shading opened=true");
	TwDefine("TweakBar/Object opened=true");
	TwDefine("TweakBar/Ambient Light opened=true");
	TwDefine("TweakBar/Light1 opened=true");
	TwDefine("TweakBar/Light2 opened=true");
	TwDefine("TweakBar/Shape opened=true");
	TwDefine("TweakBar/Camera opened=true");
	TwDefine("TweakBar/CameraSetting opened=false");
	TwDefine("TweakBar/Translate opened=false");
	TwDefine("TweakBar/Rotate opened=false");
	TwDefine("TweakBar/Scale opened=false");
	TwDefine("TweakBar/Point1 opened=false");
	TwDefine("TweakBar/Point2 opened=false");
	TwDefine("TweakBar/Point3 opened=false");
	TwDefine("TweakBar/Location opened=false");



	// Call the GLUT main loopl
	glutMainLoop();

	return 0;
}
void TW_CALL cameraTranslation(void* clientData) {
	if (result) {
		obj.ActiveCameraTranslation(cx, cy, cz);
	}
}

void TW_CALL wroldTranslation(void* clientData) {
	if (result) {
		obj.ActiveWorldTranslation(x, y, z);
	}
}
void TW_CALL objectTranslation(void* clientData) {
	if (result) {
		obj.ActiveObjectTranslation(x, y, z);

	}
}
void TW_CALL WorldRotationX(void* clientData) {
	if (result) {
		obj.ActiveWorldRotaionX(dx);
	}
}
void TW_CALL WorldRotationY(void* clientData) {
	if (result) {
		obj.ActiveWorldRotaionY(dy);
	}
}
void TW_CALL WorldRotationZ(void* clientData) {
	if (result) {
		obj.ActiveWorldRotaionZ(dz);
	}
}
void TW_CALL ObjectRotationX(void* clientData) {
	if (result) {
		obj.ActiveObjectRotaionX(dx);
	}
}
void TW_CALL ObjectRotationY(void* clientData) {
	if (result) {
		obj.ActiveObjectRotaionY(dy);
	}
}
void TW_CALL ObjectRotationZ(void* clientData) {
	if (result) {
		obj.ActiveObjectRotaionZ(dz);
	}
}
void TW_CALL WorldScale(void* clientData) {
	if (result) {
		obj.ActiveWorldScale(scale);
	}
}
void TW_CALL ObjectScale(void* clientData) {
	if (result) {
		obj.ActiveObjectScale(scale);
	}
}
void TW_CALL BBToggle(void* clientData) {
	if (result) {
		obj.BBprintflag = 1 - obj.BBprintflag;
	}
}
void TW_CALL lookAt(void* clientData) {
	if (result) {
		obj.lookAtOrigen();
	}
}

void TW_CALL ChangeCameraSetting(void* clientData) {
	if (result) {
		obj.camera.CameraType = obj.camera.CameraType - 1;
	}
}

// Updated loadOBJModel function
void TW_CALL loadOBJModel(void* data)
{
	// **Step 1: Clear existing data**
	if (!objScene.m_points.empty() || !objScene.m_normals.empty() ||
		!objScene.m_textureCoordinates.empty() || !objScene.m_faces.empty()) {
		objScene.clear();
	}

	// Clear 'obj' if it holds relevant data
	obj.clear();

	// **Step 2: Open file dialog to select a new OBJ file**
	std::wstring str = getOpenFileName();
	if (str.empty()) {
		std::cerr << "No file selected." << std::endl;
		return;
	}

	// **Step 3: Load the new OBJ file**
	result = objScene.load_file(str);
	if (result)
	{
		std::cout << "The OBJ file was loaded successfully." << std::endl;
		obj.ReadObj(objScene);
		// HW3: Additional processing if needed
	}
	else
	{
		std::cerr << "Failed to load OBJ file." << std::endl;
		return; // Early exit if loading failed
	}

	// **Step 4: Output the number of vertices and triangles**
	std::cout << "The number of vertices in the model is: " << objScene.m_points.size() << std::endl;
	std::cout << "The number of triangles in the model is: " << objScene.m_faces.size() << std::endl;
}


//do not change this function unless you really know what you are doing!
void initGraphics(int argc, char* argv[])
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
	if (err != GLEW_OK)
	{
		assert(0);
		return;
	}
}
//from buton mode we call the revent function located at Renderer.cpp
void drawScene()
{
	switch (mode) {

	case(0):
		renderer.drawline(g_P1x, g_P1y, g_P2x, g_P2y, colorRGB);
		break;
	case(1):
		renderer.drawtiangle_3P(g_P1x, g_P1y, g_P2x, g_P2y, g_P3x, g_P3y, colorRGB);
		break;
	case(2):
		renderer.drawsquere_2P(g_P1x, g_P1y, g_P2x, g_P2y, colorRGB);
		break;
	case(3):
		renderer.drawCircle_1P1R(g_P1x, g_P1y, Radius, colorRGB);
		break;
	case(4):
		renderer.drawPentagram_1P(g_P1x, g_P1y, colorRGB);
		break;

	}


}

void glUseScreenCoordinates(int width, int height)
{
	//get updated screen ratio

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



	if (result) {
		TwDefine("TweakBar/'Point1' visible=false");
		TwDefine("TweakBar/'Point2' visible=false");
		TwDefine("TweakBar/'Point3' visible=false");
		TwDefine("TweakBar/'Mode' visible=false");
		obj.BBprintflag = BBprintflag;
		obj.printVertixNormalsflag = printVertixNormalsflag;
		obj.worldAxis = ShowObjectAxis;
		obj.objectAxis = ShowWorldAxis;

		obj.shading = shadingmode;


		if (obj.shading == 0) {

			TwDefine("TweakBar/'wireframe' visible=true");
			TwDefine("TweakBar/'showCenter' visible=true");

			TwDefine("TweakBar/'objSettings' visible=false");
			TwDefine("TweakBar/'Light1' visible=false");
			TwDefine("TweakBar/'Light2' visible=false");
			TwDefine("TweakBar/'AMB' visible=false");

		}
		else {
			TwDefine("TweakBar/'wireframe' visible=false");
			TwDefine("TweakBar/'showCenter' visible=false");

			TwDefine("TweakBar/'objSettings' visible=true");
			TwDefine("TweakBar/'Light1' visible=true");
			TwDefine("TweakBar/'Light2' visible=true");
			TwDefine("TweakBar/'AMB' visible=true");

			obj.L1mode = L1mode;
			obj.L2mode = L2mode;
			obj.L2Toggle = L2Toggle;
			obj.doubleSide = doubleSide;

			obj.light.L1_pos[0] = xL1;
			obj.light.L1_pos[1] = yL1;
			obj.light.L1_pos[2] = zL1;
			obj.light.L2_pos[0] = xL2;
			obj.light.L2_pos[1] = yL2;
			obj.light.L2_pos[2] = zL2;

			obj.BufferMode = BufferMode;

		}
		if (obj.L1mode) {

			TwDefine("TweakBar/'lightdir1' visible=false");
			TwDefine("TweakBar/'X-L1' visible=true");
			TwDefine("TweakBar/'Y-L1' visible=true");
			TwDefine("TweakBar/'Z-L1' visible=true");

		}
		else {

			TwDefine("TweakBar/'lightdir1' visible=true");
			TwDefine("TweakBar/'X-L1' visible=false");
			TwDefine("TweakBar/'Y-L1' visible=false");
			TwDefine("TweakBar/'Z-L1' visible=false");
		}
		if (obj.L2mode) {

			TwDefine("TweakBar/'lightdir2' visible=false");
			TwDefine("TweakBar/'X-L2' visible=true");
			TwDefine("TweakBar/'Y-L2' visible=true");
			TwDefine("TweakBar/'Z-L2' visible=true");
		}
		else {

			TwDefine("TweakBar/'lightdir2' visible=true");
			TwDefine("TweakBar/'X-L2' visible=false");
			TwDefine("TweakBar/'Y-L2' visible=false");
			TwDefine("TweakBar/'Z-L2' visible=false");
		}




		if (obj.L2Toggle) {
			TwDefine("TweakBar/'Light2' visible=true");
		}
		else {
			TwDefine("TweakBar/'Light2' visible=false");

		}



		obj.light.amb_intencity[0] = ILA[0];
		obj.light.amb_intencity[1] = ILA[1];
		obj.light.amb_intencity[2] = ILA[2];

		obj.light.L1_intencity[0] = IL1[0];
		obj.light.L1_intencity[1] = IL1[1];
		obj.light.L1_intencity[2] = IL1[2];

		obj.light.L2_intencity[0] = IL2[0];
		obj.light.L2_intencity[1] = IL2[1];
		obj.light.L2_intencity[2] = IL2[2];



		obj.objcolor[0] = colorObj & 0xFF;
		obj.objcolor[1] = (colorObj >> 8) & 0xFF;
		obj.objcolor[2] = (colorObj >> 16) & 0xFF;
		obj.print();
		renderer.PrintLines(obj.linesToPrint, colorRGB);
		if (showCenter) { renderer.drawCircle_1P1R(obj.origenToprint[0] + renderer.screenwith, renderer.screenhight + obj.origenToprint[1], 10, colorRGB); }
		if (obj.worldAxis) {
			renderer.drawline(obj.origenToprint[0] + renderer.screenwith, renderer.screenhight + obj.origenToprint[1], obj.origenx.x + renderer.screenwith, renderer.screenhight + obj.origenx.y, 0x0ff0000);
			renderer.drawline(obj.origenToprint[0] + renderer.screenwith, renderer.screenhight + obj.origenToprint[1], obj.origeny.x + renderer.screenwith, renderer.screenhight + obj.origeny.y, 0x000ff00);
			renderer.drawline(obj.origenToprint[0] + renderer.screenwith, renderer.screenhight + obj.origenToprint[1], obj.origenz.x + renderer.screenwith, renderer.screenhight + obj.origenz.y, 0x00000ff);
		}
		if (obj.objectAxis) {
			renderer.drawline(obj.origenToprint[0] + renderer.screenwith, renderer.screenhight + obj.origenToprint[1], obj.localX.x + renderer.screenwith, renderer.screenhight + obj.localX.y, 0x00FFFF);
			renderer.drawline(obj.origenToprint[0] + renderer.screenwith, renderer.screenhight + obj.origenToprint[1], obj.localY.x + renderer.screenwith, renderer.screenhight + obj.localY.y, 0xFF00FF);
			renderer.drawline(obj.origenToprint[0] + renderer.screenwith, renderer.screenhight + obj.origenToprint[1], obj.localZ.x + renderer.screenwith, renderer.screenhight + obj.localZ.y, 0xFFFF00);
		}
	}
	else { drawScene(); }
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


	obj.screenhight = height / 4;
	obj.screenwith = width / 4;
	renderer.screenhight = height / 2;
	renderer.screenwith = width / 2;

	obj.camera.aspectRatio = float(width) / float(height);


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


