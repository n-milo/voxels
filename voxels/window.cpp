#include "window.h"

#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow *g_window;

void window::create(int width, int height, std::string title) {
	if (!glfwInit()) {
		fprintf(stderr, "Could not init GLFW.");
	}
	
	g_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
	if (!g_window) {
		fprintf(stderr, "Could not create window.");
	}
	
	glfwSwapInterval(1);
	
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
