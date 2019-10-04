#ifndef block_h
#define block_h

#define CHUNK_SIZE 16

#include <cstdint>

struct Block {
	uint8_t type;
};

struct Chunk {
	Chunk();
	~Chunk();
	Block* blocks;
};


#endif
