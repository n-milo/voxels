#include "block.h"

World::World(int size) {
	this->size = size;
	chunks = new Chunk*[size * size * size];
	FOR3(x, y, z, size) {
		chunks[x + y * size + z * size * size] = new Chunk();
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

int World::getSize() {
	return size;
}
