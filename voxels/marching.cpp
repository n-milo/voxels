#include "marching.h"

#include "marching_tables.h"

int windingOrder[3] = { 0, 1, 2 };
float cube[8];
vec3 edgeVertex[12];

float getOffset(float v1, float v2, float surface) {
	float delta = v2 - v1;
	return (delta == 0.0f) ? surface : (surface - v1) / delta;
}

void march(int x, int y, int z, World *world, std::vector<vec3> &verts, float surface) {
	int i, j, vert;
	int flagIndex = 0;
	float offset = 0.0f;
	
	// Find which vertices are inside of the surface and which are outside
	for (i = 0; i < 8; i++)
		if (cube[i] <= surface)
			flagIndex |= 1 << i;
	
	// Find which edges are intersected by the surface
	int edgeFlags = cubeEdgeFlags[flagIndex];
	
	// If the cube is entirely inside or outside of the surface, then there will be
	// no intersections
	if (edgeFlags == 0)
		return;
	
	// Find the point of intersection of the surface with each edge
	for (i = 0; i < 12; i++) {
		// if there is an intersection on this edge
		if ((edgeFlags & (1 << i)) != 0) {
			offset = getOffset(cube[edgeConnection[i][0]], cube[edgeConnection[i][1]], surface);
			
			edgeVertex[i].x = x + (vertexOffset[edgeConnection[i][0]][0] + offset * edgeDirection[i][0]);
			edgeVertex[i].y = y + (vertexOffset[edgeConnection[i][0]][1] + offset * edgeDirection[i][1]);
			edgeVertex[i].z = z + (vertexOffset[edgeConnection[i][0]][2] + offset * edgeDirection[i][2]);
		}
	}
	
	// Save the triangles that were found. There can be up to five per cube
	for (i = 0; i < 5; i++) {
		if (triangleConnectionTable[flagIndex][3 * i] < 0)
			break;
		
		for (j = 0; j < 3; j++) {
			vert = triangleConnectionTable[flagIndex][3 * i + j];
			verts.push_back(edgeVertex[vert]);
		}
	}
}

void marching::generate(World *world, int cx, int cy, int cz, std::vector<vec3> &verts, float surface) {
	if (surface > 0) {
		windingOrder[0] = 0;
		windingOrder[1] = 1;
		windingOrder[2] = 2;
	} else {
		windingOrder[0] = 2;
		windingOrder[1] = 1;
		windingOrder[2] = 0;
	}
	
	int x, y, z, i;
	int ix, iy, iz;
	for (x = cx * 16 - 1; x < cx * 16 + 16; x++) {
		for (y = cy * 16; y < cy * 16 + 16; y++) {
			for (z = cz * 16 -1; z < cz * 16 + 16; z++) {
				// Get the values in the 8 neighbours which make up a cube
				for (i = 0; i < 8; i++) {
					ix = x + vertexOffset[i][0];
					iy = y + vertexOffset[i][1];
					iz = z + vertexOffset[i][2];
					cube[i] = 0;// world->getBlockAmount(ix, iy, iz);
				}
				
				// Perform algorithm
				march(x, y, z, world, verts, surface);
			}
		}
	}
}

