#ifndef render_h
#define render_h

class Chunk;

namespace render {
	
	void drawBlock(Chunk *chunk, int x, int y, int z, float r, float g, float b);
	
	void renderChunk(Chunk *chunk);
	
}


#endif
