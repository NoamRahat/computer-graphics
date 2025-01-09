#include "Renderer.h"


Renderer::Renderer()
{
}


Renderer::~Renderer()
{
}


//this function turns on the specified pixels on screen
void Renderer::drawPixels(const std::vector<Pixel>& pixels)
{
	int numPixels = pixels.size();

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_INT, sizeof(Pixel), &pixels[0].x);

	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, sizeof(Pixel), &pixels[0].color);

	glDrawArrays(GL_POINTS, 0, numPixels);
}

void Renderer::drawtiangle_3P(int x0, int y0, int x1, int y1,int x2,int y2, unsigned int colorB) {
	//draw line to every point
	drawline(x0, y0, x2, y2, colorB);
	drawline(x1, y1, x2, y2, colorB);
	drawline(x0, y0, x1, y1, colorB);
}

void Renderer::drawsquere_2P(int x0, int y0, int x1, int y1, unsigned int colorB) {
	//creat the other 2 points
	int x2 = x0;
	int y2 = y1;
	int x3 = x1;
	int y3 = y0;
	//draw line to every point
	drawline(x0, y0, x2, y2, colorB);
	drawline(x1, y1, x2, y2, colorB);
	drawline(x3, y3, x1, y1, colorB);
	drawline(x0, y0, x3, y3, colorB);
}

void Renderer::drawCircle_1P1R(int x0, int y0, int radius, unsigned int colorB)
{
	std::vector<Pixel> pixels;


	int x = 0;
	int y = radius;
	int d = 1 -  radius;

	while (x <= y)
	{
		//add pixels for all eight octants
		pixels.push_back({ x0 + x, y0 + y,colorB }); // Octant 1
		pixels.push_back({ x0 - x, y0 + y,colorB }); // Octant 2
		pixels.push_back({ x0 + x, y0 - y,colorB }); // Octant 3
		pixels.push_back({ x0 - x, y0 - y,colorB }); // Octant 4
		pixels.push_back({ x0 + y, y0 + x,colorB }); // Octant 5
		pixels.push_back({ x0 - y, y0 + x,colorB }); // Octant 6
		pixels.push_back({ x0 + y, y0 - x,colorB }); // Octant 7
		pixels.push_back({ x0 - y, y0 - x,colorB }); // Octant 8

		if (d < 0) //move horizontally
		{
			d += 2 * x + 3;
		}
		else //move diagonally
		{
			d += 2 * (x - y) + 5;
			y--;
		}
		x++;
	}
	this->drawPixels(pixels);

}

void Renderer::drawline(int tx0, int ty0, int tx1, int ty1, unsigned int colorB) {
	std::vector<Pixel> pixels;

	//if x0>x1 swap points for Bresenham aussumptions 
	int temp;
	int x0 = tx0, y0 = ty0, x1 = tx1, y1 = ty1;
	if (x0 > x1) {
		temp = x1;
		x1 = x0;
		x0 = temp;
		temp = y0;
		y0 = y1;
		y1 = temp;
	}
	tx0 = x0;
	ty0 = y0;
	tx1 = x1;
	ty1 = y1;


	//for the algorithem to work we need to be on the second octant so we transform all the points to the second 
	this->quadrantFix(&x0, &y0, &x1, &y1);

	//this part we coverd in class
	int dx = x1 - x0;
	int dy = y1 - y0;
	int x = x0;
	int y = y0;
	int d = 2 * dy - dx;
	int de = 2 * dy;
	int dne = 2 * (dy - dx);
	pixels.push_back({ x0,y0,colorB });
	//loop until the end point is reached
	while (x < x1)
	{
		if (d < 0) {
			d += de;
		}
		else {
			d += dne;
			y++;
		}
		x++;
		pixels.push_back({ x,y,colorB });
	}

	//from here we redo the quadrantFix for every pixel in order so it will be ready to print
	//calc the original dy and dx so we can know what transormation to do
	dy = (ty1 - ty0);
	dx = (tx1 - tx0);
	//for every pixel fix it acording to the oct 
	for (auto& pixel : pixels) {
		if (dy > dx) {//first oct swap x and y
			std::swap(pixel.x, pixel.y);
		}
		else if (dy >= 0) {//second oct do nothing
		}
		else if (dy > -dx) {//third oct y=-y
			pixel.y = -pixel.y;
		}
		else {//fourth oct y=-y then swap x and y
			std::swap(pixel.x, pixel.y);
			pixel.y = -pixel.y;
		}
	}
	//print
	this->drawPixels(pixels);

}

void Renderer::quadrantFix(int* x0, int* y0, int* x1, int* y1) {
	int dx = *x1 - *x0;
	int dy = *y1 - *y0;
	int tempX0;
	int tempY0;
	int tempX1;
	int tempY1;

	if (dy > dx) {//first oct swap x and y
		tempX0 = *y0;
		tempY0 = *x0;
		tempX1 = *y1;
		tempY1 = *x1;
	}
	

	else if (dy >= 0) {//second oct do nothing
		return;
	}
	else if (dy > -dx) {//third oct y=-y
		tempX0 = *x0;
		tempY0 = -*y0;
		tempX1 = *x1;
		tempY1 = -*y1;
	}
	else {//fourth oct y=-y then swap x and y
		tempX0 = -*y0;
		tempY0 = *x0;
		tempX1 = -*y1;
		tempY1 = *x1;
	}

	// Update the original values
	*x0 = tempX0;
	*y0 = tempY0;
	*x1 = tempX1;
	*y1 = tempY1;

}

void Renderer::drawPentagram_1P(int x0, int y0, unsigned int colorB)
{
	//creat the 5 points
	int x1 = x0;
	int x2 = x0-140;
	int x3 = x0-95;
	int x4 = x0+95;
	int x5 = x0+140;
	int y1 = y0+160;
	int y2 = y0+80;
	int y3 = y0-130;
	int y4 = y0-130;
	int y5 = y0+80;
	//draw line to every point
	drawline(x1, y1, x3, y3, colorB);
	drawline(x3, y3, x5, y5, colorB);
	drawline(x5, y5, x2, y2, colorB);
	drawline(x2, y2, x4, y4, colorB);
	drawline(x1, y1, x4, y4, colorB);
}

void Renderer::PrintLines(const std::vector<glm::vec4> linesToPrint, unsigned int colorB) {
	for (const auto& line : linesToPrint) {
		drawline(line.x + screenwith, line.y + screenhight, line.z + screenwith, line.w + screenhight, colorB);

	}
}