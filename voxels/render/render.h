#ifndef render_h
#define render_h

#include "colour.h"

class Chunk;
class World;

namespace render {
	void drawBlock(Chunk *chunk, int x, int y, int z, Colour colour);
	void renderChunk(World *world, Chunk *chunk);
}


#endif
