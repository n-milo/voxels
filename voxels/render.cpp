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

unsigned buildListOld(Chunk *chunk) {
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

static const vec3 sun = { 0, 200, 100 };

unsigned buildList(World *world, Chunk *chunk) {
	std::vector<vec3> verts;
	marching::generate(world, chunk->coords.x, chunk->coords.y, chunk->coords.z, verts);
	
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
			vec3 toLight = (sun - v).nor();
			
			float diffuse = normal.dot(toLight);
			if (diffuse < 0.0f) diffuse = 0.0f;
			float ambient = 0.05f + (normal.x * normal.x) * 0.2f;
			
			float brightness = ambient + diffuse;
			if (brightness > 1.0f) brightness = 1.0f;
			
			float r = normal.x * .5f + .5f;
			float g = normal.y * .5f + .5f;
			float b = normal.z * .5f + .5f;
			
			glColor4f(r * brightness, g * brightness, b * brightness, 1);
			glVertex3f(v.x, v.y, v.z);
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
