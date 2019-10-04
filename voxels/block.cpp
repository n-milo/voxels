#include "block.h"

#include <random>

Chunk::Chunk() {
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(0, 2);
	blocks = new Block[CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE];
	int succ = 0, tot  =0;
	for (int i = 0; i < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; i++) {
		int r = rand() % 2;
		if (r) succ++;
		tot++;
		blocks[i].type = r;
	}
	printf("%d / %d (%f)\n", succ, tot, (float) succ / (float) tot);
}

Chunk::~Chunk() {
	delete [] blocks;
}
