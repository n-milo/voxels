#ifndef window_h
#define window_h

#include <string>

namespace window {
	
	void create(int width, int height, std::string title);
	void update();
	void destroy();
	
	bool shouldClose();
	
	
}

#endif
