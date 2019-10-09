#include "render.h"

#include <iostream>
#include <OpenGL/gl.h>
#include <vector>
#include "vec3.h"
#include "block.h"
#include "marching.h"

void color(float r, float g, float b, float c) {
	glColor3f(r * c, g * c, b * c);
}

void drawFace(Chunk *chunk, int face, int x, int y, int z, float r, float g, float b) {
	// this function automatically generates faces when given a "face" value from 0-6
	// each face value corresponds to one face on a cube
	// 0 = +X, 1 = -X, 2 = +Y, 3 = -Y, 4 = +Z, 5 = -Z
	
	int axis = face % 3; // the primary axis. Face 0-1 is X, 2-3 is Y, 4-5 is Z
	int sign = (face % 2) * 2 - 1; // faces 0,2,4 have a positive normal, and 1,3,5 have a negative normal
	float shade;
	if (axis == 0) shade = .5f;
	else if (axis == 1) shade = sign > 0 ? 1.f : .4f;
	else shade = .75f;
	glColor3f(shade, shade, shade);
	
	int positions[] = { x, y, z };
	float verts[12]; // 4 vertices, each one has an x, y, z coordinate
	
	for (int i = 0; i < 4; i++) {
		// the axis coordinate is always -0.5 + position or 0.5 + position, depending on the sign of the normal
		// the sign of the other coordinates alternate in this pattern:
		// - +
		// - -
		// + -
		// + +
		// that pattern is flipped (+ becomes -) if sign is negative
		
		int a, b; // the axes other than the primary axis
		if (axis == 0) { a = 1; b = 2; }
		else if (axis == 1) { a = 2; b = 0; }
		else { a = 1; b = 0; }
		
		// p. axis coord is always the sign of the normal
		verts[i * 3 + axis] = sign * 0.5f + positions[axis];
		// a coords follow the - - + + pattern
		verts[i * 3 + a] = (i < 2 ? -1 : 1) * 0.5f + positions[a];
		// b coords follow the + - - + pattern
		verts[i * 3 + b] = ((i == 0 || i == 3) ? 1 : -1) * 0.5f + positions[b];
	}
	
	for (int i = 0; i < 4; i++) {
		// load the positions into opengl
		glVertex3f(verts[i * 3 + 0], verts[i * 3 + 1], verts[i * 3 + 2]);
	}
}

void render::drawBlock(Chunk *chunk, int x, int y, int z, float r, float g, float b) {
	for (int i = 0; i < 6; i++) {
		drawFace(chunk, i, x, y, z, r, g, b);
	}
}

unsigned buildListVoxels(World *world, Chunk *chunk) {
	GLuint index = glGenLists(1);
	glNewList(index, GL_COMPILE);
	glBegin(GL_QUADS);
	FOR3(x, y, z, CHUNK_SIZE) {
		Block block = chunk->blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE];
		if (block.type) {
			Colour blockColour = block.getBlockType().diffuse;
			render::drawBlock(chunk, x, y, z, blockColour.r, blockColour.g, blockColour.b);
		}
	}
	glEnd();
	glEndList();
	
	return index;
}

static const vec3 sun = { 0, 200, 100 };

unsigned buildListMarching(World *world, Chunk *chunk) {
	std::vector<vec3> verts;
	std::vector<Colour> colours;
	marching::generate(world, chunk->coords.x, chunk->coords.y, chunk->coords.z, verts, colours);
	
	GLuint index = glGenLists(1);
	glNewList(index, GL_COMPILE);
	glBegin(GL_TRIANGLES);
	
	for (int t = 0; t < verts.size(); t += 3) {
		vec3 v1 = verts[t + 0];
		vec3 v2 = verts[t + 1];
		vec3 v3 = verts[t + 2];
		
		vec3 p = v2 - v1;
		vec3 q = v3 - v1;
		vec3 normal = p.cross(q).nor();
		
		for (int i = 0; i < 3; i++) {
			vec3 v = i == 0 ? v1 : (i == 1 ? v2 : v3);
			Colour col = colours[t + i];
			vec3 toLight = (sun - v).nor();
			
			float diffuse = normal.dot(toLight);
			if (diffuse < 0.0f) diffuse = 0.0f;
			float ambient = 0.1f + (normal.x * normal.x) * 0.2f + (normal.z < 0 ? normal.z * normal.z : 0) * 0.1f;
			
			float brightness = ambient + diffuse;
			if (brightness > 1.0f) brightness = 1.0f;
			
//			if (normal.x > 0.8)
//				glColor4f(1, 0, 1, 1);
//			else
				glColor4f(col.r * brightness, col.g * brightness, col.b * brightness, 1);
			glVertex3f(v.x, v.y, v.z);
		}
	}
	
	glEnd();
	glEndList();
	
	return index;
}

void render::renderChunk(World *world, Chunk *chunk) {
	if (chunk->displayList == -1) {
		chunk->displayList = buildListVoxels(world, chunk);
	}
	
	glCallList(chunk->displayList);
}
