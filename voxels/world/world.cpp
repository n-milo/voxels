#include "block.h"

#include <stdexcept>
#include "noise.h"
#include <random>
#include "math.h"

World::World(int size) {
	this->size = size;
	srand((unsigned) time(0));
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

int* randArray(int length) {
	int *arr = new int[length];
	for (int i = 0; i < length; i++) {
		arr[i] = rand() % 65536;
	}
	return arr;
}

int** randArray(int l1, int l2) {
	int **arr = new int*[l1];
	for (int i = 0; i < l1; i++) {
		arr[i] = randArray(l2);
	}
	return arr;
}

#define CORNER(xx,yy,zz) noiseMap[x / 8 + xx][y / 4 + yy][z / 8 + zz]

void World::generate() {
	float noiseMap[size * CHUNK_SIZE / 8 + 1][size * CHUNK_SIZE / 4 + 1][size * CHUNK_SIZE / 8 + 1];
	
	int octaves = 1;
	int islandOctaves = 4;
	int bottomOctaves = 1;
	int *offsets = randArray((octaves + islandOctaves + bottomOctaves) * 3);
	FOR3_MAX(x, y, z, size * CHUNK_SIZE / 8 + 1, size * CHUNK_SIZE / 4 + 1, size * CHUNK_SIZE / 8 + 1) {
		noiseMap[x][y][z] = noise::octaves(1, x, y, z, 1.45, 0.95, offsets, 0.5);
	}
	
	for (int z = 0; z < size * CHUNK_SIZE; z++) {
		for (int x = 0; x < size * CHUNK_SIZE; x++) {
			float island = noise::octaves(islandOctaves, x, z, 0, offsets + (octaves * 3), 0.01f);
			if (island < 0.2) {
				continue;
			}
			
			int bottom = (int) (noise::octaves(bottomOctaves, x, z, 0, offsets + ((octaves + islandOctaves) * 3), 0.05f) * 16 + 48);
			
			for (int y = bottom; y < size * CHUNK_SIZE; y++) {
				float c000 = CORNER(0, 0, 0);
				float c001 = CORNER(0, 0, 1);
				float c010 = CORNER(0, 1, 0);
				float c011 = CORNER(0, 1, 1);
				float c100 = CORNER(1, 0, 0);
				float c101 = CORNER(1, 0, 1);
				float c110 = CORNER(1, 1, 0);
				float c111 = CORNER(1, 1, 1);
				float lx = (x % 8) / 8.0f;
				float ly = (y % 4) / 4.0f;
				float lz = (z % 8) / 8.0f;
				
				float noise = trilerp(lx, ly, lz, c000, c001, c010, c011, c100, c101, c110, c111) - ((y - 64) / 32.0);
				if (noise > 0.0) {
					setType(2, x, y, z);
				}
			}
		}
	}
	delete [] offsets;
	
	FOR3(x, y, z, size * CHUNK_SIZE) {
		if (!block(x, y, z).type) continue;
		
		if (block(x, y + 1, z).type == 0) {
			setType(1, x, y, z);
		} else {
			for (int i = 2; i <= 5; i++) {
				if (block(x, y + i, z).type == 0) {
					setType(3, x, y, z);
					break;
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
