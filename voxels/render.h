#ifndef render_h
#define render_h

class Chunk;
class World;

namespace render {
	
	void drawBlock(Chunk *chunk, int x, int y, int z, float r, float g, float b);
	
	void renderChunk(World *world, Chunk *chunk);
	
}


#endif
