#pragma once

enum ShapeMode { LINE, SQUARE, CIRCLE, TRIANGLE, STAR, PENTAGON };

namespace GlobalSettings {
    extern ShapeMode currentShapeMode;
    extern unsigned int lineColor;
    extern int lineStartX, lineStartY;
    extern int lineEndX, lineEndY;
    extern int shapeSize;
    extern int circleCenterX, circleCenterY, circleRadius;
	extern double g_Scale;
}
