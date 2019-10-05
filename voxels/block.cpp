#include "block.h"

#include <random>

Chunk::Chunk() {
	blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	
	FOR3(x, y, z, CHUNK_SIZE) {
		float fx = (x - CHUNK_SIZE / 2);
		float fy = (y - CHUNK_SIZE / 2);
		float fz = (z - CHUNK_SIZE / 2);
		bool active = sqrt(fx * fx + fy * fy + fz * fz) < 15;
		blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE].type = active;
	}
}

Chunk::~Chunk() {
	delete [] blocks;
}
