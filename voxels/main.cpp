#include <iostream>

#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#include "window.h"
#include <cmath>

void color(float r, float g, float b, float c) {
	glColor3f(r * c, g * c, b * c);
}

void drawBlock(int x, int y, int z, float r, float g, float b) {
	color(r, g, b, 0.75f);
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(1.0f + x, -1.0f + y, -1.0f + z);
	glVertex3f(-1.0f + x, -1.0f + y, -1.0f + z);
	glVertex3f(-1.0f + x, 1.0f + y, -1.0f + z);
	glVertex3f(1.0f + x, 1.0f + y, -1.0f + z);
	
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(-1.0f + x, -1.0f + y, 1.0f + z);
	glVertex3f(1.0f + x, -1.0f + y, 1.0f + z);
	glVertex3f(1.0f + x, 1.0f + y, 1.0f + z);
	glVertex3f(-1.0f + x, 1.0f + y, 1.0f + z);
	
	color(r, g, b, 0.5f);
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(1.0f + x, -1.0f + y, 1.0f + z);
	glVertex3f(1.0f + x, -1.0f + y, -1.0f + z);
	glVertex3f(1.0f + x, 1.0f + y, -1.0f + z);
	glVertex3f(1.0f + x, 1.0f + y, 1.0f + z);
	
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(-1.0f + x, -1.0f + y, -1.0f + z);
	glVertex3f(-1.0f + x, -1.0f + y, 1.0f + z);
	glVertex3f(-1.0f + x, 1.0f + y, 1.0f + z);
	glVertex3f(-1.0f + x, 1.0f + y, -1.0f + z);
	
	color(r, g, b, 1.f);
	glNormal3f(0.0f, -1.0f, 0.0f);
	glVertex3f(-1.0f + x, -1.0f + y, -1.0f + z);
	glVertex3f(1.0f + x, -1.0f + y, -1.0f + z);
	glVertex3f(1.0f + x, -1.0f + y, 1.0f + z);
	glVertex3f(-1.0f + x, -1.0f + y, 1.0f + z);
	
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(1.0f + x, 1.0f + y, -1.0f + z);
	glVertex3f(-1.0f + x, 1.0f + y, -1.0f + z);
	glVertex3f(-1.0f + x, 1.0f + y, 1.0f + z);
	glVertex3f(1.0f + x, 1.0f + y, 1.0f + z);
}

int main(int argc, const char * argv[]) {
	window::create(640, 480, "My Game");
	
	glClearColor(1, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	float t = 0;
	while (!window::shouldClose()) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glPushMatrix();
		gluPerspective(70, 640.f / 480.f, 0.1f, 1000.f);
		
		glPushMatrix();
		gluLookAt(30 * cos(t), 20, 30 * sin(t), 8, 8, 8, 0, 1, 0);
		
		glBegin(GL_QUADS);
		for (int z = 0; z < 16; z++) {
			for (int y = 0; y < 16; y++) {
				for (int x = 0; x < 16; x++) {
					drawBlock(x, y, z, 1, 1, 1);
				}
			}
		}
		
		glEnd();
		glPopMatrix();
		glPopMatrix();
		
		t += 0.001f;
		window::update();
	}
	
	window::destroy();
	
	return 0;
}
