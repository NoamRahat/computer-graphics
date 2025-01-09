#pragma once
#include <vector>
#include"camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
class BoundingBox
{
public:
	BoundingBox();
	~BoundingBox();
	std::vector<glm::vec4> BB;
	void printBB(std::vector<glm::vec4> &linesToPrint, std::vector<glm::vec4> BBPrint);

	void updateBoundingBox();
protected:

};
