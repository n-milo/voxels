#ifndef window_h
#define window_h

#include <string>

class GLFWwindow;

namespace window {
	
	void create(int width, int height, std::string title);
	void update();
	void destroy();
	
	bool shouldClose();
	float getDeltaTime();
	
	GLFWwindow* getWindow();
	
	
}

#endif
