#include "Renderer.h"
#define M_PI 3.14159265358979323846

// Draw a line using Bresenham's Algorithm
void Renderer::drawLine(int x1, int y1, int x2, int y2, unsigned int color) {
    std::vector<Pixel> pixels;

    int dx = abs(x2 - x1), dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1, sy = y1 < y2 ? 1 : -1;
    int err = dx - dy, e2;

    while (true) {
        pixels.push_back({ x1, y1, color });
        if (x1 == x2 && y1 == y2) break;

        e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 < dx) { err += dx; y1 += sy; }
    }

    drawPixels(pixels);
}

// Generate points for a regular polygon
std::vector<std::pair<int, int>> Renderer::generatePolygonPoints(int x, int y, int sides, int radius) {
    std::vector<std::pair<int, int>> points;
    double angleStep = 2 * M_PI / sides;

    for (int i = 0; i < sides; ++i) {
        double angle = i * angleStep;
        points.emplace_back(x + static_cast<int>(radius * cos(angle)),
            y - static_cast<int>(radius * sin(angle)));
    }
    return points;
}

// Draw a square
void Renderer::drawSquare(int x, int y, int size, unsigned int color) {
    auto points = generatePolygonPoints(x, y, 4, size / 2);
    for (size_t i = 0; i < points.size(); ++i) {
        auto next = (i + 1) % points.size();
        drawLine(points[i].first, points[i].second, points[next].first, points[next].second, color);
    }
}

// Draw a circle using Midpoint Circle Algorithm
void Renderer::drawCircle(int cx, int cy, int radius, unsigned int color) {
    int x = radius, y = 0;
    int radiusError = 1 - x;

    while (x >= y) {
        drawPixels({
            {cx + x, cy + y, color}, {cx - x, cy + y, color},
            {cx + x, cy - y, color}, {cx - x, cy - y, color},
            {cx + y, cy + x, color}, {cx - y, cy + x, color},
            {cx + y, cy - x, color}, {cx - y, cy - x, color}
            });
        y++;
        if (radiusError < 0) {
            radiusError += 2 * y + 1;
        }
        else {
            x--;
            radiusError += 2 * (y - x + 1);
        }
    }
}

// Draw a triangle
void Renderer::drawTriangle(int x, int y, int size, unsigned int color) {
    auto points = generatePolygonPoints(x, y, 3, size / 2);
    for (size_t i = 0; i < points.size(); ++i) {
        auto next = (i + 1) % points.size();
        drawLine(points[i].first, points[i].second, points[next].first, points[next].second, color);
    }
}

void Renderer::drawPentagon(int x, int y, int size, unsigned int color) {
    auto points = generatePolygonPoints(x, y, 5, size / 2);
    for (size_t i = 0; i < points.size(); ++i) {
        auto next = (i + 1) % points.size();
        drawLine(points[i].first, points[i].second, points[next].first, points[next].second, color);
    }
}

// Draw a star
void Renderer::drawStar(int x, int y, int size, unsigned int color) {
    int numPoints = 5; // Five-pointed star
    float angleStep = 2 * M_PI / numPoints;

    float innerRadius = size / 2.5f; // Smaller radius for inner points

    std::vector<std::pair<int, int>> points;

    // Generate points for the star
    for (int i = 0; i < 2 * numPoints; i++) {
        float radius = (i % 2 == 0) ? size : innerRadius;
        float angle = i * angleStep / 2.0f;
        int px = x + static_cast<int>(radius * cos(angle));
        int py = y - static_cast<int>(radius * sin(angle));
        points.emplace_back(px, py);
    }

    // Draw lines between consecutive points
    for (size_t i = 0; i < points.size(); i++) {
        int nextIndex = (i + 1) % points.size();
        drawLine(points[i].first, points[i].second,
            points[nextIndex].first, points[nextIndex].second, color);
    }
}

// Draw pixels to the screen
void Renderer::drawPixels(const std::vector<Pixel>& pixels) {
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_INT, sizeof(Pixel), &pixels[0].x);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Pixel), &pixels[0].color);

    glDrawArrays(GL_POINTS, 0, pixels.size());
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
}
