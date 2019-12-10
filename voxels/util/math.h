#ifndef math_h
#define math_h

inline float lerp(float a, float b, float t) {
	return (a * (1.0 - t)) + (b * t);
}

inline float trilerp(float x, float y, float z, float c000, float c001, float c010, float c011, float c100, float c101, float c110, float c111) {
	float c00 = lerp(c000, c100, x);
	float c01 = lerp(c001, c101, x);
	float c10 = lerp(c010, c110, x);
	float c11 = lerp(c011, c111, x);
	
	float c0 = lerp(c00, c10, y);
	float c1 = lerp(c01, c11, y);
	
	return lerp(c0, c1, z);
}

#endif
