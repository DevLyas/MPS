#include "material.h"
#include <vector>

#ifndef UTILS_H
#define UTILS_H

struct Vec2
{
	float x;
	float y;
};

struct Vec3
{
	float x;
	float y;
	float z;
};

bool is_occupied(const std::vector<Particle>& v, int x, int y);

#endif /* UTILS_H */
