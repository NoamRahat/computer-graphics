#include "GlobalSettings.h"

namespace GlobalSettings {
    ShapeMode currentShapeMode = LINE;
    unsigned int lineColor = 0xFFFFFF;
    int lineStartX = 50, lineStartY = 50;
    int lineEndX = 150, lineEndY = 150;
    int shapeSize = 100;
    int circleCenterX = 200, circleCenterY = 200, circleRadius = 50;
    double g_Scale = 1.0;
    //double g_quaternion[4] = {0.0, 0.0, 0.0, 1.0};
}
