#ifndef block_h
#define block_h

#define CHUNK_SIZE 32

// these macros let me very easily write 3D for loops
// x, y, z is the name of the variable for x, y, and z

// max is the max value for x, y, and z
#define FOR3(x, y, z, max) \
for (int z = 0; z < max; z++) \
for (int y = 0; y < max; y++) \
for (int x = 0; x < max; x++)

// my, my, mz are the max X, Y, and Z respectively
#define FOR3_MAX(x, y, z, mx, my, mz) \
for (int z = 0; z < mz; z++) \
for (int y = 0; y < my; y++) \
for (int x = 0; x < mx; x++)

#define BLOCK(x, y, z) blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE]

#include <cstdint>

struct Block {
	uint8_t type = 0;
};

struct Chunk {
	Chunk();
	~Chunk();
	Block* blocks;
	unsigned displayList = -1;
	
	Block getBlock(int x, int y, int z);
	void setBlock(Block block, int x, int y, int z);
	bool inRange(int x, int y, int z);
};

class World {
	Chunk **chunks;
	int size;
	
public:
	World(int size);
	~World();
	Chunk* getChunk(int x, int y, int z);
	int getSize();
	Block getBlock(int x, int y, int z);
	bool inChunkRange(int x, int y, int z);
	
};


#endif
