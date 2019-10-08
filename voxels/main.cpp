#include <iostream>
#include <OpenGL/gl.h>
#include <GLFW/glfw3.h>
#include <GLUT/GLUT.h>
#include <cmath>
#include "window.h"
#include "block.h"
#include "render.h"
#include "vec3.h"

double lastX = -1, lastY = -1;
double xAng, yAng = 0;

Colour sky = Colour(0xA1F1FF);

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
	//glfwSetCursorPosCallback(window::getWindow(), cursor_position_callback);
	
	World *world = new World(1);
	
	glClearColor(sky.r, sky.g, sky.b, 1);
	glEnable(GL_DEPTH_TEST);
	
	double time = 0;
	int frames = 0;
	
//	glEnable(GL_CULL_FACE);
//	glCullFace(GL_BACK);
//	glFrontFace(GL_CCW);

	printf("Game running\n");
	
//	btDefaultCollisionConfiguration *config = new btDefaultCollisionConfiguration;
//	btCollisionDispatcher *dispatcher = new btCollisionDispatcher(config);
//	btBroadphaseInterface *overlappingPairCache = new btDbvtBroadphase();
//	btSequentialImpulseConstraintSolver *solver = new btSequentialImpulseConstraintSolver;
//	btDiscreteDynamicsWorld *dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, config);
	
	while (!window::shouldClose()) {
		float delta = window::getDeltaTime();
		frames++;
		time += delta;
		if (time >= 1.0) {
			time -= 1.0;
			std::string title = std::to_string(frames) + " fps";
//			glfwSetWindowTitle(window::getWindow(), title.c_str());
			frames = 0;
		}
		xAng += delta * 0.5f;
		
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glPushMatrix();
		gluPerspective(70, 640.f / 480.f, 0.1f, 1000.f);
		
		glPushMatrix();
		float dist = CHUNK_SIZE;
		gluLookAt(dist * cos(xAng) + CHUNK_SIZE / 2, CHUNK_SIZE * 1.25f, dist * sin(xAng) + CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2, CHUNK_SIZE / 2, 0, 1, 0);
		
		FOR3(cx, cy, cz, world->getSize()) {
			Chunk *chunk = world->getChunk(cx, cy, cz);
			glPushMatrix();
			glTranslatef(cx * CHUNK_SIZE, cy * CHUNK_SIZE, cz * CHUNK_SIZE);
			render::renderChunk(world, chunk);
			glPopMatrix();
		}
		
		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR) {
			printf("Error %d\n", err);
		}
		
		glPopMatrix();
		glPopMatrix();
		
		window::update();
	}
	
	delete world;
	window::destroy();
	
	return 0;
}
