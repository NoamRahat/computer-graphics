#pragma once
#include <glm/glm.hpp>
class lightSourses {
public:
	lightSourses();

	~lightSourses();
	//HW3
	double light1[3] = { 0.0f, 0.0f, -1.0f};
	float L1_intencity[3] = {1,1,1};
	float L1_pos[3] = {1,1,1};
	



	double  light2[3] = {0.0f,0.0f,-1.0f};
	float L2_intencity[3] = { 1,1,1 };
	float L2_pos[3] = { 1,1,1 };
	
	void TranslateLight(bool l,float x, float y, float z);
	void colorLight(bool l, float x, float y, float z);


	float amb_intencity[3] = {1,1,1};


};