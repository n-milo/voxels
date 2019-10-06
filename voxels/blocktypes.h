#ifndef blocktypes_h
#define blocktypes_h

#include "colour.h"

struct BlockType {
	int id;
	Colour diffuse;
};

extern BlockType blocktypes[];

#endif
