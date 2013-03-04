/* Copyright © 2013 Fabian Schuiki */
#include "TerrainRenderer.h"
#include <SFML/OpenGL.hpp>


TerrainRenderer::TerrainRenderer()
{
	// Initialize a temporary terrain.
	const double ax = 0.2;
	const double ay = sqrt(3)/2 * ax;
	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			struct Node &node = nodes[x][y];
			node.p.x = ((x - 64) + 0.5 * (y & 1)) * ax;
			node.p.z = (y - 64) * ay;
			node.p.y = 0;
		}
	}

	// Calculate the normals of the terrain.
	for (int x = 0; x < 128; x++) {
		for (int y = 0; y < 128; y++) {
			struct Node &node = nodes[x][y];
			node.n.x = 0;
			node.n.z = 0;
			node.n.y = 1;
		}
	}
}

void TerrainRenderer::draw(const RenderInfo &info)
{
	// Draw the terrain.
	glBegin(GL_TRIANGLES);
	for (int x = 0; x < 127; x++) {
		for (int y = 0; y < 126; y += 2) {
			struct Node &a = nodes[x][y];
			struct Node &b = nodes[x+1][y];
			struct Node &c = nodes[x][y+1];
			struct Node &d = nodes[x+1][y+1];
			struct Node &e = nodes[x][y+2];
			struct Node &f = nodes[x+1][y+2];

			#define vertex(v) glColor3f(0, 1, 0); glNormal3f(v.n.x, v.n.y, v.n.z); glVertex3f(v.p.x, v.p.y, v.p.z)

			vertex(a);
			vertex(b);
			vertex(c);

			vertex(b);
			vertex(d);
			vertex(c);

			vertex(c);
			vertex(f);
			vertex(e);

			vertex(c);
			vertex(d);
			vertex(f);
		}
	}
	glEnd();
}