#include "noise.h"

#define STB_PERLIN_IMPLEMENTATION
#include <stb_perlin.h>

float noise::octaves(int oct, float x, float y, float z, float frequency, float amplitude, int *offsets, float scale) {
	float ampl = 1;
	float freq = 1;
	float n = 0;
	
	for (int i = 0; i < oct; i++) {
		n += stb_perlin_noise3(x * freq * scale + offsets[i * 2], y * freq * scale + offsets[i * 2 + 1], z * freq * scale + offsets[i * 2 + 2], 0, 0, 0) * ampl;
		ampl *= amplitude;
		freq *= frequency;
	}
	
	return n;
}

float noise::octaves(int oct, float x, float y, float z, int *offsets, float scale) {
	return octaves(oct, x, y, z, 2.0f, 0.5f, offsets, scale);
}

