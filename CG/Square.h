#include "Renderer.h"

class Square {
public:
    Square(int centerX, int centerY, int size, unsigned int color = 0xFFFFFFFF)
        : centerX(centerX), centerY(centerY), size(size), color(color) {
    }

    void setColor(unsigned int newColor) { color = newColor; }
    unsigned int getColor() const { return color; }

    void setCenter(int newX, int newY) {
        centerX = newX; centerY = newY;
    }
    void setSize(int newSize) {
        size = newSize;
    }

    void draw(Renderer& renderer) const {
        renderer.drawSquare(centerX, centerY, size, color);
    }

private:
    int centerX, centerY, size;// Line start and end points
    unsigned int color;         // Line color (RGBA)
};