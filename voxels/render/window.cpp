#include "window.h"

#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow *g_window;
double g_lastTime;

static void g_key_callback(GLFWwindow *win, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(win, GLFW_TRUE);
}

void window::create(int width, int height, std::string title) {
	if (!glfwInit()) {
		fprintf(stderr, "Could not init GLFW.");
	}
	
	g_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!g_window) {
		fprintf(stderr, "Could not create window.");
	}
	
	glfwSwapInterval(1);
	glfwSetKeyCallback(g_window, g_key_callback);
	
	glfwMakeContextCurrent(g_window);
}

void window::update() {
	glfwPollEvents();
	glfwSwapBuffers(g_window);
}

void window::destroy() {
	glfwDestroyWindow(g_window);
	glfwTerminate();
}

bool window::shouldClose() {
	return glfwWindowShouldClose(g_window);
}

float window::getDeltaTime() { // only call once per frame
	double time = glfwGetTime();
	float delta = (float) (time - g_lastTime);
	g_lastTime = time;
	return delta;
}

GLFWwindow* window::getWindow() {
	return g_window;
}
