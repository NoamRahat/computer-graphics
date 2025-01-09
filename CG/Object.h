#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <assert.h>
#include <freeglut/include/GL/freeglut.h>
#include <glm/glm.hpp>
#include <unordered_map>
#include "Renderer.h"
#include <glm/ext.hpp>
#include "BB.h"
#include "stdio.h"
//#include "Wavefront_obj.h"
#include "Obj Parser/wavefront_obj.h"
#include <unordered_set>
#include <algorithm>
#include "light.h"
class Object
{
public:
	Object();
	~Object();
	Wavefront_obj parser;
	BoundingBox BB;
	Camera camera;
	lightSourses light;
	Renderer renderer;

	void clear() {
		PointArray.clear();
		VectorPoints.clear();
		triangleNormals.clear();
		vertixNormal.clear();
		vertixNormalToPrint.clear();
		colorVec.clear();
		linesToPrint.clear();
	}

	std::vector<glm::vec3> polypix;

	void ComputeBarycentricCoordinates2(
		const glm::vec4& a, const glm::vec4& b, const glm::vec4& c, float& den, glm::vec3& lambda);

	void updateBoundingBox();
	void ComputeBarycentricCoordinates(
		const glm::vec4& a, const glm::vec4& b, const glm::vec4& c,
		int x, int y, glm::vec3& lambda);
	int screenwith = 940;
	int screenhight = 640;
	int printVertixNormalsflag = 0;
	bool BBprintflag = 0;
	bool worldAxis = 0;
	bool objectAxis = 0;
	bool showlines = 0;
	float BB_diamiter[3];
	glm::mat4 transform;
	glm::mat4 RotationMatrix(float radians, char axis);
	glm::mat4 TotalRotationMatrix = { //object transform
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f };

	glm::mat4 TotalScaleMatrix = { //object scale
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f };
	glm::mat4 cameraScaleMatrixnew = { //object scale from distance
	1.0f,0.0f,0.0f,0.0f,
	0.0f,1.0f,0.0f,0.0f,
	0.0f,0.0f,1.0f,0.0f,
	0.0f,0.0f,0.0f,1.0f };
	glm::vec4 localX = {100.0f, 0.0f, 0.0f, 1.0f};
	glm::vec4 localY = {0.0f, 100.0f, 0.0f, 1.0f};
	glm::vec4 localZ = {0.0f, 0.0f, 100.0f, 1.0f};
	//the total translation matrix is simply translation for origen
	glm::mat4 TranslateMatrix(float x, float y, float z);
	glm::mat4 ScaleMatrix(float x, float y, float z);
	void XRotationMatrix(float radians);
	void YRotationMatrix(float radians);
	void ZRotationMatrix(float radians);

	glm::vec4 origen;
	glm::vec4 origenx;
	glm::vec4 origeny;
	glm::vec4 origenz;
	glm::vec4 origenToprint;
	float origenScale=1;
	float origenDefultSize=5;

	std::vector<glm::vec4> PointArray;
	std::vector<glm::vec4> PointArrayPrint;
	std::vector<glm::vec3> colorVec;
	std::vector<glm::vec4> VectorPoints;
	std::vector<glm::vec3> triangleNormals;

	//read obj functions
	std::vector<float> triangleAreas;
	std::vector<glm::vec4>  vertixNormal;
	std::vector<glm::vec4>  vertixNormalToPrint;
	std::unordered_map<int, std::vector<int>> vertexAdjacency;
	std::vector<std::vector<float>> m_points;
	std::vector<glm::vec4> linesToPrint;
	
	bool L2Toggle = 0;
	bool L1mode = 0;
	bool L2mode = 0;
	bool doubleSide = 0;

	int BufferMode = 0;
	int shading;
	int n = 2;
	float objcolor[3] = { 255,255,255 };
	float Ks = 0.2f;
	float Kd = 0.3f;
	float Ka = 0.5f;
	double zBuffer[1000][1540];

	float Nsize = 5;

	void initializeZBuffer();
	void print();
	void ActiveWorldTranslation(float x, float y, float z);
	void ActiveObjectTranslation(float x, float y, float z);	
	void ActiveCameraTranslation(float x, float y, float z);	

	void ActiveWorldRotaionX(float x);
	void ActiveWorldRotaionY(float y);
	void ActiveWorldRotaionZ(float z);
	void ActiveObjectRotaionX(float x);	
	void ActiveObjectRotaionY(float y);	
	void ActiveObjectRotaionZ(float z);	
	
	void ActiveWorldScale(float scale);
	void ActiveObjectScale(float scale);
	
	void ReadObj(Wavefront_obj obj);

	void updateCamera();
	void lookAtOrigen();
	
	//there are 2 way to print the edge and the edge will be vounted as the same one to not print double
	struct Edge {
		int v1, v2;  // Indices of the vertices

		// Constructor
		Edge(int v1, int v2) {
			// Always store the smaller index first to avoid duplicates
			if (v1 < v2) {
				this->v1 = v1;
				this->v2 = v2;
			}
			else {
				this->v1 = v2;
				this->v2 = v1;
			}
		}

		// Comparison function for using in a set or unordered_set
		bool operator==(const Edge& other) const {
			return v1 == other.v1 && v2 == other.v2;
		}

		// Hash function for unordered_set
		struct Hash {
			size_t operator()(const Edge& e) const {
				return std::hash<int>()(e.v1) ^ std::hash<int>()(e.v2);
			}
		};
	};
	std::unordered_set<Edge, Edge::Hash> edgeSet;

	
protected:

};