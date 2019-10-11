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

// ix, iy, iz are the min X, Y, and Z respectively
#define FOR3_MINMAX(x, y, z, ix, iy, iz, mx, my, mz) \
for (int z = iz; z < mz; z++) \
for (int y = iy; y < my; y++) \
for (int x = ix; x < mx; x++)

#define BLOCK(x, y, z) blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE]

#include <cstdint>
#include "blocktypes.h"

struct Block {
	uint8_t type = 0;
	// to get the amount, divide by 255
	uint8_t amount = 0;
	
	BlockType getBlockType();
	float getAmount();
};

struct ChunkCoords {
	int x, y, z;
};

class World;

struct Chunk {
	Chunk(World *world, ChunkCoords coords);
	~Chunk();
	World* world;
	Block* blocks;
	ChunkCoords coords;
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
	bool inBlockRange(int x, int y, int z);
	bool inChunkRange(int x, int y, int z);
	
};


#endif
