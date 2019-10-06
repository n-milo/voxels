#include <iostream>
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <GLUT/GLUT.h>
#include <cmath>
#include "window.h"
#include "block.h"
#include "render.h"

double lastX = -1, lastY = -1;
double xAng, yAng = 0;

static void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
	if (lastX != -1 && lastY != -1 && glfwGetMouseButton(window::getWindow(), GLFW_MOUSE_BUTTON_LEFT)) {
		double dx = xpos - lastX;
		double dy = ypos - lastY;
		xAng += dx * 0.01;
		yAng += dy * 0.01;
		if (yAng > M_PI_2) yAng = M_PI_2;
		if (yAng < -M_PI_2) yAng = -M_PI_2;
	}
	
	lastX = xpos;
	lastY = ypos;
}

int main(int argc, const char * argv[]) {
	window::create(640, 480, "My Game");
	glfwSetCursorPosCallback(window::getWindow(), cursor_position_callback);
	
	World *world = new World(2);
	
	glClearColor(1, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);
	
	double time = 0;
	int frames = 0;
	
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	
	GLuint index = glGenLists(1);
	glNewList(index, GL_COMPILE);
	
	FOR3(cx, cy, cz, world->getSize()) {
		Chunk *chunk = world->getChunk(cx, cy, cz);
		glPushMatrix();
		glTranslatef(cx * CHUNK_SIZE, cy * CHUNK_SIZE, cz * CHUNK_SIZE);
		render::renderChunk(chunk);
		glPopMatrix();
	}
	
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
		float dist = CHUNK_SIZE;
		gluLookAt(dist * cos(xAng) + CHUNK_SIZE / 2, CHUNK_SIZE * 1.25f + CHUNK_SIZE * 2.5f * sin(yAng), dist * sin(xAng) + CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2, 0, 1, 0);
		
		glCallList(index);
		
		glPopMatrix();
		glPopMatrix();
		
		window::update();
	}
	
	delete world;
	glDeleteLists(index, 1);
	window::destroy();
	
	return 0;
}
