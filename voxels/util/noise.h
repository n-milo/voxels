#ifndef noise_h
#define noise_h

namespace noise {
	float octaves(int oct, float x, float y, float z, int *offsets, float scale);
	float octaves(int oct, float x, float y, float z, float frequency, float amplitude, int *offsets, float scale);
}


#endif
