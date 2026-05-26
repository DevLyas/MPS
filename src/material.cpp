#include "material.h"

void Grid::update(int width, int height)
{
	rows  = height / size;
	 cols = width  / size;
}

void Brush::update(int x, int y, int s)
{
	rect.x = (float)(x - magnitude) * s;
	rect.y = (float)(y - magnitude) * s;
	rect.w = s + 2.0f * magnitude * s;
	rect.h = s + 2.0f * magnitude * s;
}
