#include "block.h"

World::World(int size) {
	this->size = size;
	chunks = new Chunk*[size * size * size];
	FOR3(x, y, z, size) {
		chunks[x + y * size + z * size * size] = new Chunk(this, {x, y, z});
	}
}

World::~World() {
	FOR3(x, y, z, size) {
		delete chunks[x + y * size + z * size * size];
	}
	delete [] chunks;
}

Chunk* World::getChunk(int x, int y, int z) {
	return chunks[x + y * size + z * size * size];
}

bool World::inBlockRange(int x, int y, int z) {
	return x >= 0 && y >= 0 && z >= 0 && x < size * CHUNK_SIZE && y < size * CHUNK_SIZE && z < size * CHUNK_SIZE;
}

bool World::inChunkRange(int x, int y, int z) {
	return x >= 0 && y >= 0 && z >= 0 && x < size && y < size && z < size;
}

Block World::getBlock(int x, int y, int z) {
	if (!inBlockRange(x, y, z)) {
		return { 0 };
	}
	int cx = x / CHUNK_SIZE;
	int cy = y / CHUNK_SIZE;
	int cz = z / CHUNK_SIZE;
	if (!inChunkRange(cx, cy, cz)) {
		return { 0 };
	}
	int bx = x % CHUNK_SIZE;
	int by = y % CHUNK_SIZE;
	int bz = z % CHUNK_SIZE;
	return getChunk(cx, cy, cz)->getBlock(bx, by, bz);
}

int World::getSize() {
	return size;
}
