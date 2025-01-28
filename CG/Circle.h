#define CIRCLE_H

#include "Renderer.h"

class Circle {
public:
    Circle(int centerX, int centerY, int radius, unsigned int color = 0xFFFFFFFF)
        : centerX(centerX), centerY(centerY), radius(radius), color(color) {
    }

    void setColor(unsigned int newColor) { color = newColor; }
    unsigned int getColor() const { return color; }

    void setCenter(int newCenterX, int newCenterY) {
        centerX = newCenterX; centerY = newCenterY;
    }

    void setRadius(int newRadius) { radius = newRadius; }

    void draw(Renderer& renderer) const {
        renderer.drawCircle(centerX, centerY, radius, color);
    }

private:
    int centerX, centerY;       // Circle center
    int radius;                 // Circle radius
    unsigned int color;         // Circle color (RGBA)
};
