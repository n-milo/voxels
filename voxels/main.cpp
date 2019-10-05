#include <iostream>
#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#include <cmath>
#include "window.h"
#include "block.h"

void color(float r, float g, float b, float c) {
	glColor3f(r * c, g * c, b * c);
}

void drawBlock(int x, int y, int z, float r, float g, float b) {
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

int main(int argc, const char * argv[]) {
	window::create(640, 480, "My Game");
	
	glClearColor(1, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	float t = 0;
	
	Chunk chunk;
	double time = 0;
	int frames = 0;
	
	GLuint index = glGenLists(1);
	glNewList(index, GL_COMPILE);
	glBegin(GL_QUADS);
	
	FOR3(x, y, z, CHUNK_SIZE) {
		if (chunk.blocks[x + y * CHUNK_SIZE + z * CHUNK_SIZE * CHUNK_SIZE].type) {
			drawBlock(x, y, z, 1, 1, 1);
		}
	}
	
	glEnd();
	glEndList();
	
	while (!window::shouldClose()) {
		float delta = window::getDeltaTime();
		frames++;
		time += delta;
		if (time >= 1.0) {
			time -= 1.0;
			printf("%d fps\n", frames);
			frames = 0;
		}
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glPushMatrix();
		gluPerspective(70, 640.f / 480.f, 0.1f, 1000.f);
		
		glPushMatrix();
		// spins the camera around the world (with t being the angle around the world), and makes it look at the center of the world
		gluLookAt(CHUNK_SIZE * cos(t) + CHUNK_SIZE / 2, CHUNK_SIZE * 1.25f, CHUNK_SIZE * sin(t) + CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2, 0, 1, 0);
		
		glCallList(index);
		
		glPopMatrix();
		glPopMatrix();
		
		t += delta;
		window::update();
	}
	
	glDeleteLists(index, 1);
	window::destroy();
	
	return 0;
}
