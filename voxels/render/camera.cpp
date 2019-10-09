#include "camera.h"

#include <OpenGL/gl.h>
#include <GLUT/GLUT.h>
#include <GLFW/glfw3.h>
#include "window.h"

const float deg2rad = M_PI / 180.f;

Camera::Camera(float _fov, float _aspect, vec3 _pos) : fov(_fov), aspect(_aspect), position(_pos) {
}

Camera::~Camera() {
	
}

vec3 move(float amt, int dir, float rot) {
	return vec3(amt * sin(rot * deg2rad + M_PI_2 * dir), 0, amt * cos(rot * deg2rad + M_PI_2 * dir));
}

void Camera::update(float delta) {
	float speed = 15 * delta;
	float flyspeed = 15 * delta;
	
	bool up = glfwGetKey(window::getWindow(), GLFW_KEY_W), down = glfwGetKey(window::getWindow(), GLFW_KEY_S), left = glfwGetKey(window::getWindow(), GLFW_KEY_A), right = glfwGetKey(window::getWindow(), GLFW_KEY_D);
	
	if ((up ^ down) && (left ^ right)) {
		speed *= M_SQRT1_2;
	}
	
	if (glfwGetKey(window::getWindow(), GLFW_KEY_SPACE)) {
		position.y += flyspeed;
	}
	if (glfwGetKey(window::getWindow(), GLFW_KEY_LEFT_SHIFT)) {
		position.y -= flyspeed;
	}
	
	if (up) {
		position += move(-speed, 0, rotation.y);
	}
	if (down) {
		position += move(speed, 0, rotation.y);
	}
	if (left) {
		position += move(-speed, 1, rotation.y);
	}
	if (right) {
		position += move(speed, 1, rotation.y);
	}
}

void Camera::usePerspective() {
	gluPerspective(fov, aspect, 0.1f, 1000.f);
}

void Camera::useView() {
	glRotatef(-rotation.x, 1, 0, 0);
	glRotatef(-rotation.y, 0, 1, 0);
	glRotatef(-rotation.z, 0, 0, 1);
	glTranslatef(-position.x, -position.y, -position.z);
}
