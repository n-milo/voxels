#include "block.h"

#include <random>

BlockType Block::getBlockType() {
	return blocktypes[type];
}

Chunk::Chunk(World *_world, ChunkCoords _coords) : world(_world), coords(_coords) {
	blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
}

Chunk::~Chunk() {
	delete [] blocks;
}

Block Chunk::block(int x, int y, int z) {
	if (!inRange(x, y, z)) {
		return world->block(x + coords.x * CHUNK_SIZE, y + coords.y * CHUNK_SIZE, z + coords.z * CHUNK_SIZE);
	}
	return BLOCK(x, y, z);
}

void Chunk::setType(int type, int x, int y, int z) {
	if (!inRange(x, y, z)) return;
	BLOCK(x, y, z).type = type;
}

void Chunk::setBlock(Block block, int x, int y, int z) {
	if (!inRange(x, y, z)) return;
	BLOCK(x, y, z) = block;
}

bool Chunk::inRange(int x, int y, int z) {
	return x >= 0 && y >= 0 && z >= 0 && x < CHUNK_SIZE && y < CHUNK_SIZE && z < CHUNK_SIZE;
}
