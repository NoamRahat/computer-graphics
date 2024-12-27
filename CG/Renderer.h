#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <cmath>
#include <freeglut/include/GL/freeglut.h>

class Wavefront_obj;

struct Pixel
{
    int x, y; //pixel in screen coordinates
    unsigned int color; //RGBA color format - 4 components of 8 bits each - 0xAABBGGRR - AA alpha, BB blue, RR red
};

class Renderer
{
public:
    void drawLine(int x1, int y1, int x2, int y2, unsigned int color);
    void drawLine(const glm::vec3& start, const glm::vec3& end, unsigned int color);
    std::vector<std::pair<int, int>> Renderer::generatePolygonPoints(int x, int y, int sides, int radius);
    void drawSquare(int x, int y, int size, unsigned int color);
    void drawCircle(int cx, int cy, int radius, unsigned int color);
    void drawTriangle(int x, int y, int size, unsigned int color);
    void drawPentagon(int x, int y, int size, unsigned int color);
    void drawStar(int x, int y, int size, unsigned int color);
    void drawPixels(const std::vector<Pixel>& pixels);

protected:

};

void renderObject(Renderer& renderer, Wavefront_obj& obj);
