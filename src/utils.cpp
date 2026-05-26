#include "utils.h"

bool is_occupied(const std::vector<Particle> &v, int x, int y)
{
	for (const auto& p : v)
	{
		if (p.rect.x == x && p.rect.y == y) return true;
	}
	return false;
}
