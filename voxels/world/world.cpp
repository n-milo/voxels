#include "block.h"

#include <stdexcept>

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

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

void World::generate() {
	for (int z = 0; z < size * CHUNK_SIZE; z++) {
		for (int x = 0; x < size * CHUNK_SIZE; x++) {
			for (int y = size * CHUNK_SIZE - 1; y >= 0; y--) {
				float scale = 1.f / 16.f;
				if (stb_perlin_ridge_noise3(x * scale, y * scale, z * scale, 2.0, 0.5, 1.0, 6) > 0.5) {
					int type = 2;
					for (int i = 1; i <= 6; i++) {
						if (block(x, y + i, z).type == 0) {
							type = (i <= 2 ? 1 : 3);
							break;
						}
					}
					setType(type, x, y, z);
				}
			}
		}
	}
}

bool World::inBlockRange(int x, int y, int z) {
	return x >= 0 && y >= 0 && z >= 0 && x < size * CHUNK_SIZE && y < size * CHUNK_SIZE && z < size * CHUNK_SIZE;
}

bool World::inChunkRange(int x, int y, int z) {
	return x >= 0 && y >= 0 && z >= 0 && x < size && y < size && z < size;
}

Block World::block(int x, int y, int z) {
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
	return getChunk(cx, cy, cz)->block(bx, by, bz);
}

void World::setType(int type, int x, int y, int z) {
	if (!inBlockRange(x, y, z)) return;
	int cx = x / CHUNK_SIZE;
	int cy = y / CHUNK_SIZE;
	int cz = z / CHUNK_SIZE;
	if (!inChunkRange(cx, cy, cz)) return;
	int bx = x % CHUNK_SIZE;
	int by = y % CHUNK_SIZE;
	int bz = z % CHUNK_SIZE;
	getChunk(cx, cy, cz)->setType(type, bx, by, bz);
}

int World::getSize() {
	return size;
}
