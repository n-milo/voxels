#ifndef marching_h
#define marching_h

#include <vector>
#include "vec3.h"
#include "colour.h"

class World;

namespace marching {
	void generate(World *world, int cx, int cy, int cz, std::vector<vec3> &verts, std::vector<Colour> &colours, float surface = 0.5);
};

#endif

