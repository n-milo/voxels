#include "block.h"

#include <random>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

BlockType Block::getBlockType() {
	return blocktypes[type];
}

float Block::getAmount() {
	return amount / 255.f;
}

Chunk::Chunk(World *_world, ChunkCoords _coords) : world(_world), coords(_coords) {
	blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	
	FOR3(x, y, z, CHUNK_SIZE) {
		float scale = 1.f / 16.f;
		float amt = stb_perlin_ridge_noise3((coords.x * CHUNK_SIZE + x) * scale, (coords.y * CHUNK_SIZE + y) * scale, (coords.z * CHUNK_SIZE + z) * scale, 2.0, 0.5, 1.0, 6);
		BLOCK(x, y, z).amount = (uint8_t)(amt * 255);
		BLOCK(x, y, z).type = amt > 0.5 ? (y > 16 ? 1 : 2) : 0;
	}
}

Chunk::~Chunk() {
	delete [] blocks;
}

Block Chunk::getBlock(int x, int y, int z) {
	if (!inRange(x, y, z)) {
		return world->getBlock(x + coords.x * CHUNK_SIZE, y + coords.y * CHUNK_SIZE, z + coords.z * CHUNK_SIZE);
	}
	return BLOCK(x, y, z);
}

void Chunk::setBlock(Block block, int x, int y, int z) {
	if (!inRange(x, y, z)) return;
	BLOCK(x, y, z) = block;
}

bool Chunk::inRange(int x, int y, int z) {
	return x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE;
}
