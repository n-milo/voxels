#include "render.h"

#include <OpenGL/gl.h>
#include "block.h"

void color(float r, float g, float b, float c) {
	glColor3f(r * c, g * c, b * c);
}

void render::drawBlock(Chunk *chunk, int x, int y, int z, float r, float g, float b) {
	if (chunk->getBlock(x + 1, y, z).type && chunk->getBlock(x - 1, y, z).type && chunk->getBlock(x, y + 1, z).type && chunk->getBlock(x, y - 1, z).type && chunk->getBlock(x, y, z + 1).type && chunk->getBlock(x, y, z - 1).type) {
		return;
	}
	
	color(r, g, b, 0.75f);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(0.5f + x, -0.5f + y, -0.5f + z);
	glVertex3f(-0.5f + x, -0.5f + y, -0.5f + z);
	glVertex3f(-0.5f + x, 0.5f + y, -0.5f + z);
	glVertex3f(0.5f + x, 0.5f + y, -0.5f + z);
	
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-0.5f + x, -0.5f + y, 0.5f + z);
	glVertex3f(0.5f + x, -0.5f + y, 0.5f + z);
	glVertex3f(0.5f + x, 0.5f + y, 0.5f + z);
	glVertex3f(-0.5f + x, 0.5f + y, 0.5f + z);
	
	color(r, g, b, 0.5f);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.5f + x, -0.5f + y, 0.5f + z);
	glVertex3f(0.5f + x, -0.5f + y, -0.5f + z);
	glVertex3f(0.5f + x, 0.5f + y, -0.5f + z);
	glVertex3f(0.5f + x, 0.5f + y, 0.5f + z);
	
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-0.5f + x, -0.5f + y, -0.5f + z);
	glVertex3f(-0.5f + x, -0.5f + y, 0.5f + z);
	glVertex3f(-0.5f + x, 0.5f + y, 0.5f + z);
	glVertex3f(-0.5f + x, 0.5f + y, -0.5f + z);
	
	color(r, g, b, 1.f);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-0.5f + x, -0.5f + y, -0.5f + z);
	glVertex3f(0.5f + x, -0.5f + y, -0.5f + z);
	glVertex3f(0.5f + x, -0.5f + y, 0.5f + z);
	glVertex3f(-0.5f + x, -0.5f + y, 0.5f + z);
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.5f + x, 0.5f + y, -0.5f + z);
	glVertex3f(-0.5f + x, 0.5f + y, -0.5f + z);
	glVertex3f(-0.5f + x, 0.5f + y, 0.5f + z);
	glVertex3f(0.5f + x, 0.5f + y, 0.5f + z);
	
}

unsigned buildList(Chunk *chunk) {
	GLuint index = glGenLists(1);
	glNewList(index, GL_COMPILE);
	glBegin(GL_QUADS);
	FOR3(x, y, z, CHUNK_SIZE) {
		if (chunk->blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE].type) {
			render::drawBlock(chunk, x, y, z, 1, 1, 1);
		}
	}
	glEnd();
	glEndList();
	
	return index;
}

void render::renderChunk(Chunk *chunk) {
	if (chunk->displayList == -1) {
		chunk->displayList = buildList(chunk);
	}
	
	glCallList(chunk->displayList);
}
