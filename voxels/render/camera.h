#ifndef camera_h
#define camera_h

#include "vec3.h"

class Camera {
	float fov, aspect;
	
public:
	vec3 position, rotation;
	
	Camera(float fov, float aspect, vec3 pos);
	~Camera();
	void update(float delta);
	void usePerspective();
	void useView();
	
};

#endif
