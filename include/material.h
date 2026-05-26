#include <SDL3/SDL.h>

#ifndef MATERIAL_H
#define MATERIAL_H

enum class State
{
	STATIC = 0,
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

struct Grid
{
	Grid(int s, int c, int r, bool h) : size{s}, rows{r}, cols{c}, hidden{h} {};

	int size;
	int rows;
	int cols;
	bool hidden;

	void update(int w, int h);
};

struct Brush
{
	SDL_FRect rect;
	int magnitude;

	void update(int x, int y, int s);
};

struct Particle
{
	SDL_FRect rect;
	State state;
};

#endif /* MATERIAL_H */
