#include "block.h"

#include <random>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

Chunk::Chunk() {
	blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	
	FOR3(x, y, z, CHUNK_SIZE) {
		float scale = 1.f / 16.f;
		bool active = stb_perlin_ridge_noise3(x * scale, y * scale, z * scale, 2.0, 0.5, 1.0, 6) < 0.5;
		BLOCK(x, y, z).type = active;
	}
}

Chunk::~Chunk() {
	delete [] blocks;
}

int Chunk::getType(int x, int y, int z) {
	if (!inRange(x, y, z)) return 0;
	return blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE].type;
}

void Chunk::setBlock(Block block, int x, int y, int z) {
	if (!inRange(x, y, z)) return;
	blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE] = block;
}

bool Chunk::inRange(int x, int y, int z) {
	return x > 0 && y > 0 && z > 0 && x <= CHUNK_SIZE && y <= CHUNK_SIZE && z <= CHUNK_SIZE;
}
