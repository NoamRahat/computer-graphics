#include "Renderer.h"

class House {
public:
    House(int centerX, int centerY, int size, unsigned int color = 0xFFFFFFFF)
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
        renderer.drawHouse(centerX, centerY, size, color);
    }

private:
    int centerX, centerY, size;
    unsigned int color;         // Line color (RGBA)
};

