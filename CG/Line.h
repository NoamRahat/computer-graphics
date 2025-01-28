#pragma once
#include "Renderer.h"

class Line {
public:
    Line(int x1, int y1, int x2, int y2, unsigned int color = 0xFFFFFFFF)
        : x1(x1), y1(y1), x2(x2), y2(y2), color(color) {
    }

    void setColor(unsigned int newColor) { color = newColor; }
    unsigned int getColor() const { return color; }

    void setPoints(int newX1, int newY1, int newX2, int newY2) {
        x1 = newX1; y1 = newY1; x2 = newX2; y2 = newY2;
    }

    void draw(Renderer& renderer) const {
        renderer.drawLine(x1, y1, x2, y2, color);
    }

private:
    int x1, y1, x2, y2;         // Line start and end points
    unsigned int color;         // Line color (RGBA)
};