#include "BB.h"
BoundingBox::BoundingBox() {

}
BoundingBox::~BoundingBox() {

}



void BoundingBox::printBB(std::vector<glm::vec4> &linesToPrint, std::vector<glm::vec4> BBPrint) {
    if (BBPrint[0].z > -1 && BBPrint[0].z < 1) {
        if (BBPrint[1].z > -1 && BBPrint[1].z < 1)
            linesToPrint.emplace_back(round(BBPrint[0][0]), round(BBPrint[0][1]), round(BBPrint[1][0]), round(BBPrint[1][1]));
        if (BBPrint[2].z > -1 && BBPrint[2].z < 1)
            linesToPrint.emplace_back(round(BBPrint[0][0]), round(BBPrint[0][1]), round(BBPrint[2][0]), round(BBPrint[2][1]));
        if (BBPrint[4].z > -1 && BBPrint[4].z < 1)
            linesToPrint.emplace_back(round(BBPrint[0][0]), round(BBPrint[0][1]), round(BBPrint[4][0]), round(BBPrint[4][1]));
    }
    if (BBPrint[1].z > -1 && BBPrint[1].z < 1) {
        if (BBPrint[3].z > -1 && BBPrint[3].z < 1)
            linesToPrint.emplace_back(round(BBPrint[1][0]), round(BBPrint[1][1]), round(BBPrint[3][0]), round(BBPrint[3][1]));
        if (BBPrint[5].z > -1 && BBPrint[5].z < 1)
            linesToPrint.emplace_back(round(BBPrint[1][0]), round(BBPrint[1][1]), round(BBPrint[5][0]), round(BBPrint[5][1]));
    }
    if (BBPrint[2].z > -1 && BBPrint[2].z < 1) {
        if (BBPrint[3].z > -1 && BBPrint[3].z < 1)
            linesToPrint.emplace_back(round(BBPrint[2][0]), round(BBPrint[2][1]), round(BBPrint[3][0]), round(BBPrint[3][1]));
        if (BBPrint[6].z > -1 && BBPrint[6].z < 1)
            linesToPrint.emplace_back(round(BBPrint[2][0]), round(BBPrint[2][1]), round(BBPrint[6][0]), round(BBPrint[6][1]));
    }
    if (BBPrint[3].z > -1 && BBPrint[3].z < 1) {
        if (BBPrint[7].z > -1 && BBPrint[7].z < 1)
            linesToPrint.emplace_back(round(BBPrint[3][0]), round(BBPrint[3][1]), round(BBPrint[7][0]), round(BBPrint[7][1]));
    }
    if (BBPrint[4].z > -1 && BBPrint[4].z < 1) {
        if (BBPrint[5].z > -1 && BBPrint[5].z < 1)
            linesToPrint.emplace_back(round(BBPrint[4][0]), round(BBPrint[4][1]), round(BBPrint[5][0]), round(BBPrint[5][1]));
        if (BBPrint[6].z > -1 && BBPrint[6].z < 1)
            linesToPrint.emplace_back(round(BBPrint[4][0]), round(BBPrint[4][1]), round(BBPrint[6][0]), round(BBPrint[6][1]));
    }
    if (BBPrint[5].z > -1 && BBPrint[5].z < 1) {
        if (BBPrint[7].z > -1 && BBPrint[7].z < 1)
            linesToPrint.emplace_back(round(BBPrint[5][0]), round(BBPrint[5][1]), round(BBPrint[7][0]), round(BBPrint[7][1]));
    }
    if (BBPrint[6].z > -1 && BBPrint[6].z < 1) {
        if (BBPrint[7].z > -1 && BBPrint[7].z < 1)
            linesToPrint.emplace_back(round(BBPrint[6][0]), round(BBPrint[6][1]), round(BBPrint[7][0]), round(BBPrint[7][1]));
    }
		}

	
