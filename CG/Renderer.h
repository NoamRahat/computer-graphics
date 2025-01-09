#pragma once

#include <vector>
#include <freeglut/include/GL/freeglut.h>
#include <glm/glm.hpp>
#include <iostream>
struct Pixel
{
	int  x, y; //pixel in screen coordinates
	unsigned int color; //RGBA color format - 4 components of 8 bits each - 0xAABBGGRR - AA alpha, BB blue, RR red
};

class Renderer
{
public:
	Renderer();
	~Renderer();
	int screenwith=750;
	int screenhight=375;


	void drawPixels(const std::vector<Pixel>& pixels);
	void drawline(int x0, int y0,int x1, int y1, unsigned int colorB);
	void quadrantFix(int *x0, int *y0, int *x1, int *y1);
	void drawsquere_2P(int x0, int y0, int x1, int y1, unsigned int colorB);
	void drawtiangle_3P(int x0, int y0, int x1, int y1, int x2, int y2, unsigned int colorB);
	void drawCircle_1P1R(int x0, int y0,int radius, unsigned int colorB);
	void drawPentagram_1P(int x0, int y0, unsigned int colorB);
	void PrintLines(const std::vector<glm::vec4> linesToPrint, unsigned int colorB);
protected:

};
