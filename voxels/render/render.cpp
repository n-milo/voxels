#include "render.h"

#include <iostream>
#include <OpenGL/gl.h>
#include <vector>
#include "vec3.h"
#include "block.h"
#include "marching.h"

const float aoIntensity = 0.5f;
const float aoCurve[] = { 0.f, .6f, .8f, 1.f };

void color(float r, float g, float b, float c) {
	glColor3f(r * c, g * c, b * c);
}

int vertexAO(bool side1, bool side2, bool corner) {
	if (side1 && side2) {
		return 0;
	}
	return 3 - (side1 + side2 + corner);
}

int getAO(Chunk *chunk, int x, int y, int z, int axis, int a, int b, int sign, int aDir, int bDir) {
	Block side1, side2, corner;
	int checkCoords[] = {
		0, 0, 0, 0, 0, 0, 0, 0, 0
	}; // the 9 positions (3 blocks) to check
	for (int i = 0; i < 3; i++) {
		checkCoords[i * 3 + axis] = sign;
		if (i == 0 || i == 2) // only check a for side1 or corner
			checkCoords[i * 3 + a] = aDir;
		if (i == 1 || i == 2) // only check b for side2 or corner
			checkCoords[i * 3 + b] = bDir;
	}
	side1  = chunk->block(x + checkCoords[0], y + checkCoords[1], z + checkCoords[2]);
	side2  = chunk->block(x + checkCoords[3], y + checkCoords[4], z + checkCoords[5]);
	corner = chunk->block(x + checkCoords[6], y + checkCoords[7], z + checkCoords[8]);
	return vertexAO(side1.type, side2.type, corner.type);
}

void drawFace(Chunk *chunk, int face, int x, int y, int z, Colour colour) {
	// this function automatically generates faces when given a "face" value from 0-6
	// each face value corresponds to one face on a cube
	// 0 = +X, 1 = -X, 2 = +Y, 3 = -Y, 4 = +Z, 5 = -Z
	
	int axis = face / 2; // the primary axis. Face 0-1 is X, 2-3 is Y, 4-5 is Z
	int sign = (face % 2) * 2 - 1; // faces 0,2,4 have a positive normal, and 1,3,5 have a negative normal
	int a, b; // the axes other than the primary axis
	int positions[] = { x, y, z };
	
	if (axis == 0) { a = 1; b = 2; }
	else if (axis == 1) { a = 2; b = 0; }
	else { a = 1; b = 0; }
	
	// only render if this face is touching an air block
	int checkBlock[3];
	checkBlock[axis] = positions[axis] + sign;
	checkBlock[a] = positions[a];
	checkBlock[b] = positions[b];
	if (chunk->block(checkBlock[0], checkBlock[1], checkBlock[2]).type) {
		return;
	}
	
	// calculate basic shading
	float shade;
	if (axis == 0) shade = .5f;
	else if (axis == 1) shade = sign > 0 ? 1.f : .4f;
	else shade = .75f;
	glNormal3f((axis == 0) * sign, (axis == 1) * sign, (axis == 2) * sign);
	
	float verts[12], colours[12];
	int aos[4];
	
	for (int i = 0; i < 4; i++) {
		// the axis coordinate is always -0.5 + position or 0.5 + position, depending on the sign of the normal
		// the sign of the other coordinates alternate in this pattern:
		// - +
		// - -
		// + -
		// + +
		// that pattern is flipped (+ becomes -) if sign is negative
		
		// the sign of A and B
		// a coords follow the - - + + pattern
		// b coords follow the + - - + pattern
		int aDir = i < 2 ? -sign : sign;
		int bDir = (i == 0 || i == 3) ? sign : -sign;
		
		int aoValue = getAO(chunk, x, y, z, axis, a, b, sign, aDir, bDir);
		aos[i] = aoValue;
		float ao = aoCurve[aoValue];
		ao = ao * aoIntensity + (1 - aoIntensity);
		for (int j = 0; j < 3; j++) {
			colours[i * 3 + j] = ao * shade * colour.raw[j];
		}
		
		// p. axis coord is always the sign of the normal
		verts[i * 3 + axis] = sign * 0.5f + positions[axis];
		verts[i * 3 + a] = aDir * 0.5f + positions[a];
		verts[i * 3 + b] = bDir * 0.5f + positions[b];
	}
	
	// send the data to opengl
	for (int i = 0; i < 4; i++) {
		glColor3f(colours[i * 3 + 0], colours[i * 3 + 1], colours[i * 3 + 2]);
		glVertex3f(verts[i * 3 + 0], verts[i * 3 + 1], verts[i * 3 + 2]);
	}
}

void render::drawBlock(Chunk *chunk, int x, int y, int z, Colour colour) {
	for (int i = 0; i < 6; i++) {
		drawFace(chunk, i, x, y, z, colour);
	}
}

unsigned buildList(World *world, Chunk *chunk) {
	GLuint index = glGenLists(1);
	glNewList(index, GL_COMPILE);
	glBegin(GL_QUADS);
	FOR3(x, y, z, CHUNK_SIZE) {
		Block block = chunk->blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE];
		if (block.type) {
			render::drawBlock(chunk, x, y, z, block.getBlockType().diffuse);
		}
	}
	glEnd();
	glEndList();
	
	return index;
}

void render::renderChunk(World *world, Chunk *chunk) {
	if (chunk->displayList == -1) {
		chunk->displayList = buildList(world, chunk);
	}
	
	glCallList(chunk->displayList);
}
